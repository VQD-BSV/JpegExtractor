import os, time

def extract_jpeg_segments(input_file_path, output_folder):

    print("---------------------Repairing---------------------")
    time.sleep(2)

    number = 0
    with open(input_file_path.strip('"'), 'rb') as file:
        file_content = file.read()

    start_marker = b'\xFF\xD8\xFF'
    end_marker = b'\xFF\xD9'

    start_pos = file_content.find(start_marker)

    while start_pos != -1:
        end_pos = file_content.find(end_marker, start_pos + len(start_marker))
        if end_pos != -1:
            jpeg_data = file_content[start_pos:end_pos + len(end_marker)]

            print(f"-------0x{start_pos:X}_0x{end_pos:X}-------")
            time.sleep(1)
            # Create the output folder if it doesn't exist
            os.makedirs(output_folder, exist_ok=True)
            number = number + 1
            output_file_path = os.path.join(output_folder, f"IMG_{number}.JPG")
            with open(output_file_path, 'wb') as output_file:
                output_file.write(jpeg_data)
            
            # Find the next segment
            start_pos = file_content.find(start_marker, end_pos + len(end_marker))
        else:
            # If end_marker is not found, exit the loop
            break

    print("---------------------Done---------------------")
    time.sleep(2)
    
if __name__ == "__main__":
    print("JpegExtract_v1.0 Copyright (c) 2023 Vũ Quang Đại (Bevis Vũ)")
    input_file_path = input("File Path: ").strip()
    output_folder = input("Folder Repaired: ").strip()

    extract_jpeg_segments(input_file_path, output_folder)
