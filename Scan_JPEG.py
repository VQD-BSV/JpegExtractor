drive_path = "\\\\.\\E:"  # Thay đổi đường dẫn đến ổ đĩa E của bạn
file_size = 512
signature_start = b'\xff\xd8\xff\xdb\x00\x84'
signature_end = b'\xff\xd9'

with open(drive_path, "rb") as drive_file:
    offset = 0
    recovery_mode = False
    recovered_file_id = 0

    while True:
        drive_file.seek(offset * file_size)
        byte = drive_file.read(file_size)

        if not byte:
            break

        found_start = byte.find(signature_start)
        if found_start >= 0:
            recovery_mode = True
            print(f'==== Found JPG at location: {hex(found_start + (file_size * offset))} ====')

            # Tạo tệp đã phục hồi và tìm chữ ký kết thúc
            recovered_file_name = f'recovered_{recovered_file_id}.jpg'
            recovered_file_id += 1
            recovered_file = open(recovered_file_name, "wb")
            recovered_file.write(byte[found_start:])

            while recovery_mode:
                byte = drive_file.read(file_size)
                found_end = byte.find(signature_end)

                if found_end >= 0:
                    recovered_file.write(byte[:found_end + 2])
                    drive_file.seek((offset + 1) * file_size)
                    print(f'==== Wrote JPG to location: {recovered_file_name} ====\n')
                    recovery_mode = False
                    recovered_file.close()
                else:
                    recovered_file.write(byte)

        offset += 1
