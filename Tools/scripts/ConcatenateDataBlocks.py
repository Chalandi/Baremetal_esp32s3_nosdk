import argparse
import struct
import hashlib

ESP_CHECKSUM_MAGIC = 0xEF  # Initial checksum value

def parse_hex_file(file):
    blocks = []
    current_block = None
    high_address = 0  # Upper 16 bits of address (from ELA records)

    with open(file, 'r') as f:
        for line in f:
            line = line.strip()
            if not line.startswith(':'):
                continue

            byte_count = int(line[1:3], 16)
            address = int(line[3:7], 16)
            record_type = int(line[7:9], 16)
            data = bytes.fromhex(line[9:9 + (byte_count * 2)])

            if record_type == 0:  # Data Record
                full_address = (high_address << 16) | address

                # If it's a new block or not contiguous, start a new block
                if current_block is None or full_address != current_block['start'] + current_block['length']:
                    if current_block:
                        blocks.append(current_block)  # Store previous block
                    current_block = {'start': full_address, 'length': 0, 'data': bytearray()}

                # Append data to current block
                current_block['data'].extend(data)
                current_block['length'] += byte_count

            elif record_type == 4:  # Extended Linear Address Record
                high_address = int(line[9:13], 16)  # Update high address

    if current_block:
        blocks.append(current_block)  # Store the last block

    return blocks

def checksum(data, state=ESP_CHECKSUM_MAGIC):
    """Calculate ESP-IDF style checksum on data block only (excluding headers)."""
    for b in data:
        state ^= b  # XOR each byte with the current state
    return state

def calculate_checksum(blocks):
    """Calculate checksum on all blocks except the first special block."""
    checksum_value = ESP_CHECKSUM_MAGIC
    for i, block in enumerate(blocks):
        if i == 0:
            continue  # Skip first special block
        checksum_value = checksum(block['data'], checksum_value)
    return checksum_value

def calculate_sha256(data):
    """Calculate SHA-256 hash of the data."""
    sha256 = hashlib.sha256()
    sha256.update(data)
    return sha256.digest()

def write_binary_with_headers(hex_file, output_file):
    blocks = parse_hex_file(hex_file)

    with open(output_file, 'wb') as outfile:
        for i, block in enumerate(blocks):
            if i == 0:
                # First block (special case): write data directly, no header
                outfile.write(block['data'])
            else:
                # Write header (start address + length, little-endian)
                outfile.write(struct.pack('<II', block['start'], block['length']))
                # Write actual block data
                outfile.write(block['data'])

    # Calculate checksum based on data-only blocks (excluding headers)
    final_checksum = calculate_checksum(blocks)

    # Append 15 zero bytes and then write the checksum byte
    with open(output_file, 'ab') as outfile:
        outfile.write(b'\x00' * 15 + struct.pack('<B', final_checksum))

    # Reopen the file to calculate the SHA-256 hash and append it at the end
    with open(output_file, 'r+b') as outfile:
        file_data = outfile.read()
        sha256_hash = calculate_sha256(file_data)

        # Write the SHA-256 hash (32 bytes) at the end of the file
        outfile.seek(0, 2)  # Move the file pointer to the end of the file
        outfile.write(sha256_hash)

def main():
    parser = argparse.ArgumentParser(description="Convert HEX file to a binary file with headers, skipping the first block header, adding ESPTool-style XOR checksum, and appending SHA-256 hash.")
    parser.add_argument("hex_file", help="The input HEX file")
    parser.add_argument("output_file", help="The output binary file")
    args = parser.parse_args()

    write_binary_with_headers(args.hex_file, args.output_file)


if __name__ == "__main__":
    main()
