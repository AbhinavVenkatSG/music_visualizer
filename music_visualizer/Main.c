#include "raylib.h"
#include <stdio.h>
#include <stdlib.h> 

#define MAX_FILEPATH_RECORDED 2
#define MAX_FILE_SIZE 1024

int main(void) {
    const int w_width = 800;
    const int w_height = 450;

    InitWindow(w_width, w_height, "Music Vizulaizer");

    int filepathCounter = 0;
    char* filepath[MAX_FILEPATH_RECORDED] = { 0 };

    for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
        filepath[i] = (char*)calloc(MAX_FILE_SIZE, 1);
        if (filepath[i] == NULL) {
            printf("Memory allocation failed for filepath[%d]\n", i);
            return -1;
        }
    }

    InitAudioDevice();

    SetTargetFPS(60);

    Sound sound = { 0 };
    bool soundLoaded = false;

    while (!WindowShouldClose()) {
        if (IsFileDropped()) {
            FilePathList droppedFiles = LoadDroppedFiles();

            if (filepathCounter >= MAX_FILEPATH_RECORDED) {
                printf("Too many files dropped.\n");
            }
            else {
                TextCopy(filepath[filepathCounter], droppedFiles.paths[0]);

                if (FileExists(filepath[filepathCounter])) {
                    sound = LoadSound(filepath[filepathCounter]);

                    if (sound.frameCount == 0) {
                        printf("ERROR: Failed to load sound from %s\n", filepath[filepathCounter]);
                    }
                    else {
                        printf("SUCCESS: Loaded sound: %s\n", filepath[filepathCounter]);
                        PlaySound(sound);
                        soundLoaded = true;
                    }

                    filepathCounter++;
                }
                else {
                    printf("ERROR: File does not exist: %s\n", filepath[filepathCounter]);
                }
            }

            UnloadDroppedFiles(droppedFiles);
        }

        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        if (filepathCounter == 0) {
            DrawText("Drag and drop your music file here", 100, 40, 20, BLACK);
        }
        else {
            DrawText("File dropped. Playing sound...", 100, 40, 20, BLACK);
        }

        EndDrawing();
    }

    if (soundLoaded) {
        UnloadSound(sound);
    }

    CloseAudioDevice();

    for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
        free(filepath[i]);
    }

    CloseWindow();
    return 0;
}