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


int max(int a, int b) {
    if (a > b)
        return a;
    else
        return b;
}

int samePixel (Pixel p1, Pixel p2) {
    if (p1.r == p2.r &&
        p1.g == p2.g &&
        p1.b == p2.b)
        return 1;
    else
        return 0;
}

int minorHeightBlur(int h, int blurSize, int row) {
  if (h - 1 > row + blurSize/2)
    return row + blurSize/2;
  else
    return h - 1;
}

int minWeightBlur(int w, int blurSize, int column) {
  if (w - 1 > column + blurSize/2)
    return column + blurSize/2;
  else
    return w - 1;
}

Image greyScale(Image img) {
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            int media = img.pixel[i][j].r + img.pixel[i][j].g + img.pixel[i][j].b;
            media /= 3;
            img.pixel[i][j].r = media;
            img.pixel[i][j].g = media;
            img.pixel[i][j].b = media;
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
          int menor_r = (255 >  p) ? p : 255;
          img.pixel[x][j].r = menor_r;

          p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
          menor_r = (255 >  p) ? p : 255;
          img.pixel[x][j].g = menor_r;

          p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
          menor_r = (255 >  p) ? p : 255;
          img.pixel[x][j].b = menor_r;
      }
  }
}

void blur (Image img, int blurSize) {
    for (unsigned int row = 0; row < img.h; ++row) {
        for (unsigned int column = 0; column < img.w; ++column) {
            Pixel media = {0, 0, 0};

            int minorHeight = minorHeightBlur (img.h, blurSize, row);
            int minWeigh = minWeightBlur (img.w, blurSize, column);

            for(int x = (0 > row - blurSize/2 ? 0 : row - blurSize/2); x <= minorHeight; ++x) {
                for(int y = (0 > column - blurSize/2 ? 0 : column - blurSize/2); y <= minWeigh; ++y) {
                    media.r += img.pixel[row][column].r;
                    media.g += img.pixel[row][column].g;
                    media.b += img.pixel[row][column].b;
                }
            }
            media.r /= blurSize * blurSize;
            media.g /= blurSize * blurSize;
            media.b /= blurSize * blurSize;

            img.pixel[row][column].r = media.r;
            img.pixel[row][column].g = media.g;
            img.pixel[row][column].b = media.b;
        }
    }
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

Image mirroried (Image img, int horizontal) {
  Image mirrored;
  int w = mirrored.w, h = mirrored.h;

  if (horizontal == 1)
    w /= 2;
  else
    h /= 2;

  for (int i2 = 0; i2 < h; ++i2) {
      for (int j = 0; j < w; ++j) {
          int x = i2, y = j;

          if (horizontal == 1)
            y = mirrored.w - 1 - j;
          else
            x = mirrored.h - 1 - i2;

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

void colorInversion(Image img) {
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            img.pixel[i][j].r = 255 - img.pixel[i][j].r;
            img.pixel[i][j].g = 255 - img.pixel[i][j].g;
            img.pixel[i][j].b = 255 - img.pixel[i][j].b;
        }
    }
}

Image cropImage(Image img, int x, int y, int w, int h) {
    Image cropped;

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


int main() {
    Image img;

    // Read type of image
    char p3[4];
    scanf("%s", p3);

    // Read width height and color of image
    int max_color;
    scanf("%u %u %d", &img.w, &img.h, &max_color);

    // Read all pixels of image
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j].r, &img.pixel[i][j].g, &img.pixel[i][j].b);
        }
    }

    int nOptions;
    scanf("%d", &nOptions);

    for(int i = 0; i < nOptions; ++i) {
        int option;
        scanf("%d", &option);

        switch(option) {
            case 1: { // Grey Scale
                img = greyScale(img);
                break;
            }
            case 2: { // Sepia Filter


                break;
            }
            case 3: { // Blur
                int size = 0;
                scanf("%d", &size);
                blur(img, size);
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
                int horizontal = 0;
                scanf("%d", &horizontal);

                mirroried(img, horizontal);
                break;
            }
            case 6: { // Color Inversion
                colorInversion(img);
                break;
            }
            case 7: { // Crop Image
                int x, y, w, h;

                scanf("%d %d", &x, &y);
                scanf("%d %d", &w, &h);

                img = cropImage(img, x, y, w, h);
                break;
            }
        }
    }

    // Print type of image
    printf("P3\n");

    // Print width height and color of image
    printf("%u %u\n255\n", img.w, img.h);

    // Print pixels of image
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j].r, img.pixel[i][j].g, img.pixel[i][j].b);
        }
        printf("\n");
    }
    return 0;
}
