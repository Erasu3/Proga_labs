#include <stdio.h>
#include <map>
#include <vector>
#include <string.h>
#include <typeinfo>


using namespace std;

int* getData(char *, int &, int &, map <int, int> &);
void RleAlg(int *, int, int, map <int, int> , char *);
void Encode(char * , char *);
void Decode(char *, char *);

int main() {
        int choice = 0;

        printf("1. Encode\n");
        printf("2. Decode\n");

        scanf("%d", &choice);

        if (choice == 1) {
                Encode("input.txt", "output.rle");
        }
        else {
                Decode("output.rle", "res.txt");
        }
        return 0;
}

void Encode(char *inName, char *outName) {
        int height, width;
        map <int, int> colorId;
        int *picture = getData(inName, height, width, colorId);

        if (picture == NULL)
                return;

        RleAlg(picture, height, width, colorId, outName);

        delete[] picture;
}

void Decode(char *inName, char *outName) {
        int palette_size = 0;

        FILE *input = fopen(inName, "rt");
        fscanf(input, "%d", &palette_size);

        int *palette = new int[palette_size];
        for (int i = 0; i < palette_size; i++)
                fscanf(input, "%d", &palette[i]);

        int height, width;
        fscanf(input, "%d %d", &width, &height);

        int *picture = new int[height * width];
        int id, cnt;
        int k = 0;

        while (!feof(input)) {
                fscanf(input, "%d ", &id);
                if (id > 127) {
                        id -= 128;
                        fscanf(input, "%d", &cnt);
                        cnt++;
                }
                else
                        cnt = 1;

                for (int i = 0; i < cnt; i++) {
                        picture[k] = palette[id];
                        k++;
                }
        }

        FILE *output = fopen(outName, "wt");
        for (int i = 0; i < height * width; i++) {
                fprintf(output, "%d ", picture[i]);
                if ((i + 1) % width == 0)
                        fprintf(output, "\n");
        }

        fclose(output);
        fclose(input);

        delete[] picture;
        delete[] palette;
}

int* getData(char* inName, int &h, int &w, map <int, int> &colorId) {
        FILE *input = fopen(inName, "rt");

        if (input == NULL) {
                printf("Error opening\n");
                return NULL;
        }

        fscanf(input, "%d %d", &w, &h);

        int *arr = new int[w * h];

        int id = 0;
        for (int i = 0; i < w * h; i++) {
                fscanf(input, "%d ", &arr[i]);
                if (!colorId.count(arr[i])) {
                        if (arr[i] > 127) {
                                printf("Error. Color must be less than 128.\n");
                                delete[] arr;
                                return NULL;
                        }
                        colorId[arr[i]] = id;
                        id++;
                }
        }

        fclose(input);

        return arr;
}

void RleAlg(int *arr, int h, int w, map <int, int> colorId, char* outName) {
        int cnt = 0;

        FILE *out = fopen(outName, "wt");
        fprintf(out, "%d\n", colorId.size());

        for ( map<int, int>::iterator it = colorId.begin(); it != colorId.end(); it++)
                fprintf(out, "%d\n", it->first);
        fprintf(out, "%d %d\n", w, h);

        for (int i = 0; i < h * w - 1; i++) {
                cnt++;
                if (arr[i] != arr[i + 1]) {
                        if (cnt == 1)
                                fprintf(out, "%d ", colorId[arr[i]]);
                        else
                                fprintf(out, "%d %d ", colorId[arr[i]] + 128, cnt - 1);
                        cnt = 0;
                }
        }

        if (arr[h * w - 1] != arr[h * w - 2])
                fprintf(out, "%d\n", colorId[arr[h * w - 1]]);
}
