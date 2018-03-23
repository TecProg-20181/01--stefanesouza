#include <stdio.h>

typedef struct _pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    Pixel pixel[512][512];
    unsigned int w;
    unsigned int h;
} Image;


int samePixel (Pixel p1, Pixel p2) {
    if (p1.r == p2.r && p1.g == p2.g && p1.b == p2.b)
        return 1;
    else
        return 0;
}

int max (int numA, int numB) {
    return (numA > numB) ? numA : numB;
}

int min (int numA, int numB) {
  return (numA < numB) ? numA : numB;
}


Image greyScale(Image img) {
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            int average = img.pixel[i][j].r + img.pixel[i][j].g + img.pixel[i][j].b;
            average /= 3;
            img.pixel[i][j].r = average;
            img.pixel[i][j].g = average;
            img.pixel[i][j].b = average;
        }
      }
    return img;
}

Image sepiaFilter(Image img) {
  for (unsigned int x = 0; x < img.h; ++x) {
    for (unsigned int j = 0; j < img.w; ++j) {
        unsigned short int pixel[3];
        pixel[0] = img.pixel[x][j].r;
        pixel[1] = img.pixel[x][j].g;
        pixel[2] = img.pixel[x][j].b;

        int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
        int min_r = min(255, p);
        img.pixel[x][j].r = min_r;

        p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
        min_r = min(255, p);
        img.pixel[x][j].g = min_r;

        p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
        min_r = min(255, p);
        img.pixel[x][j].b = min_r;
    }
  }
  return img;
}

Image blur(Image img) {
    int blurSize = 0;
    printf("Size: \n");
    scanf("%d", &blurSize);

    for (unsigned int row = 0; row < img.h; ++row) {
        for (unsigned int column = 0; column < img.w; ++column) {
            Pixel average = {0, 0, 0};

            int minHeight = min((img.h - 1), (row + blurSize/2));
            int minWeigh = min((img.w - 1), (column + blurSize/2));

            for(int x = (0 > row - blurSize/2 ? 0 : row - blurSize/2); x <= minHeight; ++x) {
                for(int y = (0 > column - blurSize/2 ? 0 : column - blurSize/2); y <= minWeigh; ++y) {
                    average.r += img.pixel[row][column].r;
                    average.g += img.pixel[row][column].g;
                    average.b += img.pixel[row][column].b;
                }
            }
            average.r /= blurSize * blurSize;
            average.g /= blurSize * blurSize;
            average.b /= blurSize * blurSize;

            img.pixel[row][column].r = average.r;
            img.pixel[row][column].g = average.g;
            img.pixel[row][column].b = average.b;
        }
    }
    return img;
}

Image rotate90(Image img) {
  Image rotation;

  rotation.w = img.h;
  rotation.h = img.w;

  for (unsigned int i = 0, y = 0; i < rotation.h; ++i, ++y) {
    for (int j = rotation.w - 1, x = 0; j >= 0; --j, ++x) {
      rotation.pixel[i][j].r = img.pixel[x][y].r;
      rotation.pixel[i][j].g = img.pixel[x][y].g;
      rotation.pixel[i][j].b = img.pixel[x][y].b;
    }
  }
  return rotation;
}

Image mirroried(Image img) {
  Image mirrored;
  int horizontal = 0;
  printf("Horizontal: \n");
  scanf("%d", &horizontal);

  int w = mirrored.w, h = mirrored.h;
  (horizontal == 1) ? (w /= 2) : (h /= 2);

  for (int i2 = 0; i2 < h; ++i2) {
      for (int j = 0; j < w; ++j) {
          int x = i2, y = j;

          (horizontal == 1) ? (y = mirrored.w - 1 - j) : (x = mirrored.h - 1 - i2);

          Pixel aux1;
          aux1.r = mirrored.pixel[i2][j].r;
          aux1.g = mirrored.pixel[i2][j].g;
          aux1.b = mirrored.pixel[i2][j].b;

          mirrored.pixel[i2][j].r = mirrored.pixel[x][y].r;
          mirrored.pixel[i2][j].g = mirrored.pixel[x][y].g;
          mirrored.pixel[i2][j].b = mirrored.pixel[x][y].b;

          mirrored.pixel[x][y].r = aux1.r;
          mirrored.pixel[x][y].g = aux1.g;
          mirrored.pixel[x][y].b = aux1.b;
      }
  }
  return mirrored;
}

Image colorInversion(Image img) {
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            img.pixel[i][j].r = 255 - img.pixel[i][j].r;
            img.pixel[i][j].g = 255 - img.pixel[i][j].g;
            img.pixel[i][j].b = 255 - img.pixel[i][j].b;
        }
    }
    return img;
}

Image cropImage(Image img) {
    Image cropped;
    int x, y, w, h;

    scanf("%d %d", &x, &y);
    scanf("%d %d", &w, &h);

    cropped.w = w;
    cropped.h = h;

    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            cropped.pixel[i][j].r = img.pixel[i + y][j + x].r;
            cropped.pixel[i][j].g = img.pixel[i + y][j + x].g;
            cropped.pixel[i][j].b = img.pixel[i + y][j + x].b;
        }
    }
    return cropped;
}

void readImageDescription(Image img) {
  // Type
  char p3[4];
  scanf("%s", p3);

  int max_color;
  scanf("%u %u %d", &img.w, &img.h, &max_color);

  // Pixels of image
  for (unsigned int i = 0; i < img.h; ++i) {
      for (unsigned int j = 0; j < img.w; ++j) {
          scanf("%hu %hu %hu", &img.pixel[i][j].r, &img.pixel[i][j].g, &img.pixel[i][j].b);
      }
  }
}

void printImageDescription(Image img) {
  printf("P3\n");
  printf("%u %u\n255\n", img.w, img.h);
  for (unsigned int i = 0; i < img.h; ++i) {
    for (unsigned int j = 0; j < img.w; ++j) {
        printf("%hu %hu %hu ", img.pixel[i][j].r, img.pixel[i][j].g, img.pixel[i][j].b);
    }
    printf("\n");
  }
}


int main() {
    Image img;

    readImageDescription(img);

    int nOptions;
    scanf("%d", &nOptions);

    printf("1. Grey Scale\n2. Sepia\n3. Blur\n4. Rotation\n5. Mirroried\n6. Color Inversion\n7. Crop Image\n");

    for(int i = 0; i < nOptions; ++i) {
        int option;
        scanf("%d", &option);

        switch(option) {
            case 1: { // Grey Scale
                img = greyScale(img);
                break;
            }
            case 2: { // Sepia Filter
              img = sepiaFilter(img);
                break;
            }
            case 3: { // Blur
                img = blur(img);
                break;
            }
            case 4: { // Rotation
                int numRotations = 0;
                scanf("%d", &numRotations);
                numRotations %= 4;
                for (int j = 0; j < numRotations; ++j) {
                    img = rotate90(img);
                }
                break;
            }
            case 5: { // Mirroried
                img = mirroried(img);
                break;
            }
            case 6: { // Color Inversion
                img = colorInversion(img);
                break;
            }
            case 7: { // Crop Image
                img = cropImage(img);
                break;
            }
        }
    }
    printImageDescription(img);
    return 0;
}
