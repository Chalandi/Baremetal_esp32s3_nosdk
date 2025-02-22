import re
import sys

def format_linker_errors(filename):
    try:
        with open(filename, 'r') as file:
            buffer = ""  # To store multi-line error messages
            error_messages = []  # Store formatted messages to find the max length later

            for line in file:
                line = line.strip()
                
                # Check if the line ends with a colon, indicating it should concatenate with the next line
                if buffer and buffer.endswith(':'):
                    buffer += " " + line  # Append to the existing buffer with a space
                    continue
                
                # If there's a buffered message, process it before continuing
                if buffer:
                    formatted_msg = process_error_message(buffer)  # Format the message
                    if formatted_msg:  # Only store non-None messages
                        error_messages.append(formatted_msg)
                
                # Reset buffer and start a new message
                buffer = line  # Start a new buffer for the current line

            # Don't forget to process the last buffered message
            if buffer:
                formatted_msg = process_error_message(buffer)
                if formatted_msg:  # Only store non-None messages
                    error_messages.append(formatted_msg)

        # Align and print messages after processing all lines
        max_length = max(len(msg.split(' (')[0]) for msg in error_messages)  # Find max length of messages without file info
        for msg in error_messages:
            base_msg, file_info = msg.rsplit(' (', 1)  # Split into message and file info
            file_info = f'({file_info}'  # Reattach the opening parenthesis for alignment
            print(f"{base_msg:<{max_length}} {file_info}")  # Align the base message

    except FileNotFoundError:
        print(f"Error: File '{filename}' not found.")
    except Exception as e:
        print(f"An error occurred: {e}")

def process_error_message(buffer):
    # Updated regex to capture the undefined reference error message
    match = re.search(
        r"function `(?P<function_name>[\w_]+)':\s*(?P<file_name>[^:]+):(?P<line_number>\d+):\(\.text\+[^)]+\): undefined reference to `(?P<undefined_reference>[\w_]+)'",
        buffer
    )

    if match:
        # Extract relevant details
        undefined_reference = match.group("undefined_reference")
        function_name = match.group("function_name")
        file_name = match.group("file_name")
        line_number = match.group("line_number")

        # Return the formatted error message
        formatted_error = f"undefined reference to `{undefined_reference}'"
        file_line_info = f" ({file_name}:{line_number})"  # Keep file and line separate
        return f"{formatted_error}{file_line_info}"
    
    return None  # Return None if no match is found


# Main execution starts here
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script_name.py <input_filename>")
    else:
        input_filename = sys.argv[1]
        format_linker_errors(input_filename)
