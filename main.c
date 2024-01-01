#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void create_folder(const char *folder_path) {
    // Check if the folder exists, and if not, create it
    #ifdef _WIN32
        _mkdir(folder_path);
    #else
        mkdir(folder_path, 0777);
    #endif
}

void extract_jpeg_segments(const char *input_file_path, const char *output_folder) {
    printf("---------------------Repairing---------------------\n");
    sleep(2);

    int number = 0;
    FILE *file = fopen(input_file_path, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    unsigned char *file_content = (unsigned char *)malloc(file_size);
    if (file_content == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return;
    }

    fread(file_content, 1, file_size, file);
    fclose(file);

    unsigned char start_marker[] = {0xFF, 0xD8, 0xFF};
    unsigned char end_marker[] = {0xFF, 0xD9};

    long start_pos = -1;
    for (long i = 0; i < file_size - 2; i++) {
        if (memcmp(&file_content[i], start_marker, sizeof(start_marker)) == 0) {
            start_pos = i;
            break;
        }
    }

    while (start_pos != -1) {
        long end_pos = -1;
        for (long i = start_pos + sizeof(start_marker); i < file_size - 1; i++) {
            if (memcmp(&file_content[i], end_marker, sizeof(end_marker)) == 0) {
                end_pos = i;
                break;
            }
        }

        if (end_pos != -1) {
            unsigned char *jpeg_data = (unsigned char *)malloc(end_pos - start_pos + sizeof(end_marker));
            if (jpeg_data == NULL) {
                perror("Error allocating memory");
                free(file_content);
                return;
            }

            memcpy(jpeg_data, &file_content[start_pos], end_pos - start_pos + sizeof(end_marker));

            printf("-------0x%lX_0x%lX-------\n", start_pos, end_pos);
            sleep(1);

            create_folder(output_folder);

            char output_file_path[100];
            sprintf(output_file_path, "%s/IMG_%d.JPG", output_folder, ++number);

            FILE *output_file = fopen(output_file_path, "wb");
            if (output_file == NULL) {
                perror("Error creating output file");
                free(jpeg_data);
                free(file_content);
                return;
            }

            fwrite(jpeg_data, 1, end_pos - start_pos + sizeof(end_marker), output_file);
            fclose(output_file);

            free(jpeg_data);

            start_pos = -1;
            for (long i = end_pos + sizeof(end_marker); i < file_size - 2; i++) {
                if (memcmp(&file_content[i], start_marker, sizeof(start_marker)) == 0) {
                    start_pos = i;
                    break;
                }
            }
        } else {
            // If end_marker is not found, exit the loop
            break;
        }
    }

    printf("---------------------Done---------------------\n");
    sleep(2);

    free(file_content);
}

int main() {
    printf("JpegExtract_v1.0 Copyright (c) 2023 Vu Quang Dai (Bevis Vu)\n");
    char input_file_path[256];
    char output_folder[256];

    printf("File Path: ");
    fgets(input_file_path, sizeof(input_file_path), stdin);
    input_file_path[strcspn(input_file_path, "\n")] = '\0';

    printf("Folder Repaired: ");
    fgets(output_folder, sizeof(output_folder), stdin);
    output_folder[strcspn(output_folder, "\n")] = '\0';

    extract_jpeg_segments(input_file_path, output_folder);

    return 0;
}
