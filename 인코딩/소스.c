#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include <math.h>

const char* encoding(int enco, int mode) {
	if (mode == 1) {
		if (enco <= 53) return "0000";
		else if (enco <= 72) return "0001";
		else if (enco <= 86) return "0010";
		else if (enco <= 96) return "0011";
		else if (enco <= 104) return "0100";
		else if (enco <= 114) return "0101";
		else if (enco <= 124) return "0110";
		else if (enco <= 136) return "0111";
		else if (enco <= 150) return "1000";
		else if (enco <= 163) return "1001";
		else if (enco <= 175) return "1010";
		else if (enco <= 186) return "1011";
		else if (enco <= 196) return "1100";
		else if (enco <= 208) return "1101";
		else if (enco <= 231) return "1110";
		else if (enco <= 255) return "1111";
	}
	else if (mode == 2) {
		if (enco <= 69) return  "000";
		else if (enco <= 92) return  "001";
		else if (enco <= 114) return  "010";
		else if (enco <= 137) return  "011";
		else if (enco <= 159) return  "100";
		else if (enco <= 181) return  "101";
		else if (enco <= 205) return  "110";
		else if (enco <= 255) return  "111";
	}
}

int main() {
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;

	//파일1
	FILE* inputFile = NULL;
	inputFile = fopen("O3.bmp", "rb");
	//inputFile = fopen("AICenterY_Dark.bmp", "rb");
	if (inputFile == NULL) {
		printf("no file");
		return 0;

	}
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);


	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	int size = bmpInfo.biSizeImage;
	int bitCnt = bmpInfo.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n", width, stride, height, size, bitCnt);

	unsigned char* inputImg = NULL, * outputImg = NULL;
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg, sizeof(unsigned char), size, inputFile);

	/*

	//파일2

	FILE* inputFile2 = NULL;
	//inputFile2 = fopen("AICenterY_CombinedNoise.bmp", "rb");
	inputFile2 = fopen("AICenterY_Org.bmp", "rb");
	if (inputFile2 == NULL) {
		printf("no file");
		return 0;
	}
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile2);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile2);

	unsigned char* inputImg2 = NULL;
	inputImg2 = (unsigned char*)calloc(size2, sizeof(unsigned char));
	fread(inputImg2, sizeof(unsigned char), size2, inputFile2);

	*/

	//HE
	int N[17] = { 0 }, tmp = 0, onoff = 0;

	FILE* outputFile = fopen("bitstream.txt", "wb");

	double* Y1 = (double*)calloc(width * height, sizeof(double));
	double* Y2 = (double*)calloc(width * height, sizeof(double));
	double* Y3 = (double*)calloc(width * height, sizeof(double));
	double* Y4 = (double*)calloc(width * height, sizeof(double));

	int enco = 0, enco1 = 0, dot[4] = { 0 }, high = 0, low = 0, high2 = 0, low2 = 0, button = 0;
	char bits[5] = { 0 };
	int mean = 0, a = 4, b = 0, c = 0;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y1[j * width + i] = (double)inputImg[j * stride + 3 * i];
			enco1 = (int)Y1[j * width + i];
			//Y2[j * width + i] = (double)enco;
			//tmp = (int)inputImg[j * stride + 3 * i];
			//N[tmp] += 1;
			//fprintf(outputFile, "000");
			if (enco1 <= 68) enco1 = 53;
			else if (enco1 <= 93) enco1 = 82;
			else if (enco1 <= 115) enco1 = 105;
			else if (enco1 <= 137) enco1 = 126;
			else if (enco1 <= 159) enco1 = 148;//
			else if (enco1 <= 181) enco1 = 170;//
			else if (enco1 <= 203) enco1 = 192;
			else if (enco1 <= 255) enco1 = 214;
			Y2[j * width + i] = (double)enco1;

			enco = (int)Y1[j * width + i];
			if (enco <= 52) enco = 43;
			else if (enco <= 71) enco = 62;//
			else if (enco <= 86) enco = 81;
			else if (enco <= 96) enco = 92;//
			else if (enco <= 104) enco = 100;
			else if (enco <= 114) enco = 109;
			else if (enco <= 124) enco = 119;
			else if (enco <= 136) enco = 130;
			else if (enco <= 150) enco = 143;
			else if (enco <= 163) enco = 157;
			else if (enco <= 175) enco = 170;
			else if (enco <= 186) enco = 181;//
			else if (enco <= 196) enco = 191;
			else if (enco <= 211) enco = 203;
			else if (enco <= 228) enco = 219;
			else if (enco <= 255) enco = 237;
			Y3[j * width + i] = (double)enco;
		}
	}

	for (int j = 1; j < height; j += 2) {
		for (int i = 1; i < width; i += 2) {
			//Y2[j * width + i] = (double)enco;
			//tmp = (int)inputImg[j * stride + 3 * i];
			//N[tmp] += 1;
			if (i == 1 || j == 1) {
				dot[0] = (int)Y3[j * width + i];
				dot[1] = (int)Y2[(j - 1) * width + (i - 1)];
				dot[2] = (int)Y2[(j - 1) * width + i];
				dot[3] = (int)Y2[j * width + (i - 1)];
				Y4[j * width + i] = Y3[j * width + i];
				Y4[(j - 1) * width + (i - 1)] = Y2[(j - 1) * width + (i - 1)];
				Y4[(j - 1) * width + i] = Y2[(j - 1) * width + i];
				Y4[j * width + (i - 1)] = Y2[j * width + (i - 1)];
				strcpy(bits, encoding(dot[0], 1));
				fprintf(outputFile, bits);
				N[1]++;
				for (int k = 1; k < 4; k++) {
					strcpy(bits, encoding(dot[k], 2));
					fprintf(outputFile, bits);
				}
			}
			else {
				dot[0] = (int)Y3[j * width + i];
				dot[1] = (int)Y3[(j - 2) * width + (i - 2)];
				dot[2] = (int)Y3[(j - 2) * width + i];
				dot[3] = (int)Y3[j * width + (i - 2)];
				if (dot[0] == dot[2]) {
					fprintf(outputFile, "00");
					button = 0;
				}
				else if (dot[0] == dot[3]) {
					fprintf(outputFile, "01");
					button = 0;
				}
				else {
					fprintf(outputFile, "1");
					button = 1;
				}
				high = dot[0];
				low = dot[0];
				for (int k = 1; k < 4; k++) {
					if (dot[k] > high) {
						high = dot[k];
					}
					else if (dot[k] < low) {
						low = dot[k];
					}
				}

				if ((high - low) == 0) {
					dot[1] = (int)Y3[(j - 1) * width + (i - 1)];
					dot[2] = (int)Y3[(j - 1) * width + i];
					dot[3] = (int)Y3[j * width + (i - 1)];
					high2 = dot[0];
					low2 = dot[0];
					for (int k = 1; k < 4; k++) {
						if (dot[k] > high2) {
							high2 = dot[k];
						}
						else if (dot[k] < low2) {
							low2 = dot[k];
						}
					}

					if ((high2 - low2) == 0) {
						if (button == 1) {
							strcpy(bits, encoding(dot[0], 1));
							fprintf(outputFile, bits);
						}
						fprintf(outputFile, "0");
						N[2]++;
						Y4[j * width + i] = Y3[j * width + i];
						Y4[(j - 1) * width + (i - 1)] = Y3[(j - 1) * width + (i - 1)];
						Y4[(j - 1) * width + i] = Y3[(j - 1) * width + i];
						Y4[j * width + (i - 1)] = Y3[j * width + (i - 1)];
					}
					else {
						dot[0] = (int)Y3[j * width + i];
						dot[1] = (int)Y2[(j - 1) * width + (i - 1)];
						dot[2] = (int)Y2[(j - 1) * width + i];
						dot[3] = (int)Y2[j * width + (i - 1)];
						if (button == 1) {
							strcpy(bits, encoding(dot[0], 1));
							fprintf(outputFile, bits);
						}
						fprintf(outputFile, "1");
						N[3]++;
						for (int k = 1; k < 4; k++) {
							strcpy(bits, encoding(dot[k], 2));
							fprintf(outputFile, bits);
						}
						Y4[j * width + i] = Y3[j * width + i];
						Y4[(j - 1) * width + (i - 1)] = Y2[(j - 1) * width + (i - 1)];
						Y4[(j - 1) * width + i] = Y2[(j - 1) * width + i];
						Y4[j * width + (i - 1)] = Y2[j * width + (i - 1)];
					}
				}
				else if ((high - low) <= 14) {
					dot[0] = (int)Y3[j * width + i];
					dot[1] = (int)Y3[(j - 1) * width + (i - 1)];
					dot[2] = (int)Y3[(j - 1) * width + i];
					dot[3] = (int)Y3[j * width + (i - 1)];

					high2 = dot[0];
					low2 = dot[0];
					for (int k = 1; k < 4; k++) {
						if (dot[k] > high2) {
							high2 = dot[k];
						}
						else if (dot[k] < low2) {
							low2 = dot[k];
						}
					}
					if ((high >= high2) && (low <= low2)) {
						if (button == 1) {
							strcpy(bits, encoding(dot[0], 1));
							fprintf(outputFile, bits);
						}
						fprintf(outputFile, "0");
						N[4]++;
						//
						mean = (high + low) / 2;
						dot[1] = (int)Y1[(j - 1) * width + (i - 1)];
						dot[2] = (int)Y1[(j - 1) * width + i];
						dot[3] = (int)Y1[j * width + (i - 1)];
						Y4[j * width + i] = Y3[j * width + i];
						if (((dot[1] - (mean - a)) * (dot[1] - (mean - a))) <= ((dot[1] - (mean + a)) * (dot[1] - (mean + a)))) {
							fprintf(outputFile, "0");
							Y4[(j - 1) * width + (i - 1)] = (double)(mean - a);
						}
						else {
							fprintf(outputFile, "1");
							Y4[(j - 1) * width + (i - 1)] = (double)(mean + a);
						}
						if (((dot[2] - (mean - a)) * (dot[2] - (mean - a))) <= ((dot[2] - (mean + a)) * (dot[2] - (mean + a)))) {
							fprintf(outputFile, "0");
							Y4[(j - 1) * width + i] = (double)(mean - a);
						}
						else {
							fprintf(outputFile, "1");
							Y4[(j - 1) * width + i] = (double)(mean + a);
						}
						if (((dot[3] - (mean - a)) * (dot[3] - (mean - a))) <= ((dot[3] - (mean + a)) * (dot[3] - (mean + a)))) {
							fprintf(outputFile, "0");
							Y4[j * width + (i - 1)] = (double)(mean - a);
						}
						else {
							fprintf(outputFile, "1");
							Y4[j * width + (i - 1)] = (double)(mean + a);
						}

						//
						//fprintf(outputFile, "0");
						//fprintf(outputFile, "1");
						//fprintf(outputFile, "0");
					}
					else {
						dot[0] = (int)Y3[j * width + i];
						dot[1] = (int)Y2[(j - 1) * width + (i - 1)];
						dot[2] = (int)Y2[(j - 1) * width + i];
						dot[3] = (int)Y2[j * width + (i - 1)];
						if (button == 1) {
							strcpy(bits, encoding(dot[0], 1));
							fprintf(outputFile, bits);
						}
						fprintf(outputFile, "1");
						N[5]++;
						for (int k = 1; k < 4; k++) {
							strcpy(bits, encoding(dot[k], 2));
							fprintf(outputFile, bits);
						}
						Y4[j * width + i] = Y3[j * width + i];
						Y4[(j - 1) * width + (i - 1)] = Y2[(j - 1) * width + (i - 1)];
						Y4[(j - 1) * width + i] = Y2[(j - 1) * width + i];
						Y4[j * width + (i - 1)] = Y2[j * width + (i - 1)];
						//fprintf(outputFile, "010");
						//fprintf(outputFile, "010");
						//fprintf(outputFile, "010");
					}
				}
				else if ((high - low) <= 38) {
					dot[0] = (int)Y3[j * width + i];
					dot[1] = (int)Y3[(j - 1) * width + (i - 1)];
					dot[2] = (int)Y3[(j - 1) * width + i];
					dot[3] = (int)Y3[j * width + (i - 1)];

					high2 = dot[0];
					low2 = dot[0];
					for (int k = 1; k < 4; k++) {
						if (dot[k] > high2) {
							high2 = dot[k];
						}
						else if (dot[k] < low2) {
							low2 = dot[k];
						}
					}
					if ((high >= high2) && (low <= low2)) {
						if (button == 1) {
							strcpy(bits, encoding(dot[0], 1));
							fprintf(outputFile, bits);
						}
						fprintf(outputFile, "0");
						mean = (high + low) / 2;
						dot[1] = (int)Y1[(j - 1) * width + (i - 1)];
						dot[2] = (int)Y1[(j - 1) * width + i];
						dot[3] = (int)Y1[j * width + (i - 1)];
						Y4[j * width + i] = Y3[j * width + i];
						if ((high - low) <= 30) { b = 4; c = 13; }
						else if ((high - low) <= 38) { b = 5; c = 17; }
						N[6]++;
						//
						if (((dot[1] - (mean - b)) * (dot[1] - (mean - b))) <= ((dot[1] - (mean + b)) * (dot[1] - (mean + b)))) {
							fprintf(outputFile, "0");
							if (((dot[1] - (mean - b)) * (dot[1] - (mean - b))) <= ((dot[1] - (mean - c)) * (dot[1] - (mean - c)))) {
								fprintf(outputFile, "1");
								Y4[(j - 1) * width + (i - 1)] = (double)(mean - b);
							}
							else {
								fprintf(outputFile, "0");
								Y4[(j - 1) * width + (i - 1)] = (double)(mean - c);
							}
						}
						else {
							fprintf(outputFile, "1");
							if (((dot[1] - (mean + b)) * (dot[1] - (mean + b))) <= ((dot[1] - (mean + c)) * (dot[1] - (mean + c)))) {
								fprintf(outputFile, "0");
								Y4[(j - 1) * width + (i - 1)] = (double)(mean + b);
							}
							else {
								fprintf(outputFile, "1");
								Y4[(j - 1) * width + (i - 1)] = (double)(mean + c);
							}
						}
						if (((dot[2] - (mean - b)) * (dot[2] - (mean - b))) <= ((dot[2] - (mean + b)) * (dot[2] - (mean + b)))) {
							fprintf(outputFile, "0");
							if (((dot[2] - (mean - b)) * (dot[2] - (mean - b))) <= ((dot[2] - (mean - c)) * (dot[2] - (mean - c)))) {
								fprintf(outputFile, "1");
								Y4[(j - 1) * width + i] = (double)(mean - b);
							}
							else {
								fprintf(outputFile, "0");
								Y4[(j - 1) * width + i] = (double)(mean - c);
							}
						}
						else {
							fprintf(outputFile, "1");
							if (((dot[2] - (mean + b)) * (dot[2] - (mean + b))) <= ((dot[2] - (mean + c)) * (dot[2] - (mean + c)))) {
								fprintf(outputFile, "0");
								Y4[(j - 1) * width + i] = (double)(mean + b);
							}
							else {
								fprintf(outputFile, "1");
								Y4[(j - 1) * width + i] = (double)(mean + c);
							}
						}
						if (((dot[3] - (mean - b)) * (dot[3] - (mean - b))) <= ((dot[3] - (mean + b)) * (dot[3] - (mean + b)))) {
							fprintf(outputFile, "0");
							if (((dot[3] - (mean - b)) * (dot[3] - (mean - b))) <= ((dot[3] - (mean - c)) * (dot[3] - (mean - c)))) {
								fprintf(outputFile, "1");
								Y4[j * width + (i - 1)] = (double)(mean - b);
							}
							else {
								fprintf(outputFile, "0");
								Y4[j * width + (i - 1)] = (double)(mean - c);
							}
						}
						else {
							fprintf(outputFile, "1");
							if (((dot[3] - (mean + b)) * (dot[3] - (mean + b))) <= ((dot[3] - (mean + c)) * (dot[3] - (mean + c)))) {
								fprintf(outputFile, "0");
								Y4[j * width + (i - 1)] = (double)(mean + b);
							}
							else {
								fprintf(outputFile, "1");
								Y4[j * width + (i - 1)] = (double)(mean + c);
							}
						}
						//@@@@@@@@@@@@@add
						//fprintf(outputFile, "01");
						//fprintf(outputFile, "01");
						//fprintf(outputFile, "01");
					}
					else {
						dot[0] = (int)Y3[j * width + i];
						dot[1] = (int)Y2[(j - 1) * width + (i - 1)];
						dot[2] = (int)Y2[(j - 1) * width + i];
						dot[3] = (int)Y2[j * width + (i - 1)];
						if (button == 1) {
							strcpy(bits, encoding(dot[0], 1));
							fprintf(outputFile, bits);
						}
						fprintf(outputFile, "1");
						N[7]++;
						for (int k = 1; k < 4; k++) {
							strcpy(bits, encoding(dot[k], 2));
							fprintf(outputFile, bits);
						}
						Y4[j * width + i] = Y3[j * width + i];
						Y4[(j - 1) * width + (i - 1)] = Y2[(j - 1) * width + (i - 1)];
						Y4[(j - 1) * width + i] = Y2[(j - 1) * width + i];
						Y4[j * width + (i - 1)] = Y2[j * width + (i - 1)];
						//fprintf(outputFile, "010");
						//fprintf(outputFile, "010");
						//fprintf(outputFile, "010");
					}
				}
				else {
					dot[0] = (int)Y3[j * width + i];
					dot[1] = (int)Y2[(j - 1) * width + (i - 1)];
					dot[2] = (int)Y2[(j - 1) * width + i];
					dot[3] = (int)Y2[j * width + (i - 1)];
					if (button == 1) {
						strcpy(bits, encoding(dot[0], 1));
						fprintf(outputFile, bits);
					}
					N[8]++;
					for (int k = 1; k < 4; k++) {
						strcpy(bits, encoding(dot[k], 2));
						fprintf(outputFile, bits);
					}
					Y4[j * width + i] = Y3[j * width + i];
					Y4[(j - 1) * width + (i - 1)] = Y2[(j - 1) * width + (i - 1)];
					Y4[(j - 1) * width + i] = Y2[(j - 1) * width + i];
					Y4[j * width + (i - 1)] = Y2[j * width + (i - 1)];
				}
			}
			//Y3[j * width + i] = (double)enco;
			//outputImg[j * stride + 3 * i + 0] = (unsigned char)Y3[j * width + i];
			//outputImg[j * stride + 3 * i + 1] = (unsigned char)Y3[j * width + i];
			//outputImg[j * stride + 3 * i + 2] = (unsigned char)Y3[j * width + i];
		}
	}

	for (int i = 1; i < 9; i++) {
		printf("%d: %d ", i, N[i]);
	}

	/*
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y3[j * width + i] = Y1[((j / 2) * 2 + 1) * width + ((i / 2) * 2 + 1)];
		}
	}*/

	/*
	int sum = 0;
	for (int i = 0; i < 256; i++) {
		sum = 0;
		for (int j = 0; j <= i; j++) {
			sum += N[j];
		}
		sumHist[i] = sum;
	}*/


	int max = 0, min = 0, cat = 0, max2 = 0, min2 = 0;
	double Y, mse = 0, psnr, mse2 = 0, psnr2;

	int dif[256] = { 0 };
	int difper[256] = { 0 };
	printf("check point1\n");

	//Y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));

	for (int j = 3; j < height; j += 2) {
		for (int i = 3; i < width; i += 2) {
			/*Y = 0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0];
			Cb = -0.169 * inputImg[j * stride + 3 * i + 2] - 0.331 * inputImg[j * stride + 3 * i + 1] + 0.500 * inputImg[j * stride + 3 * i + 0];
			Cr = 0.500 * inputImg[j * stride + 3 * i + 2] - 0.419 * inputImg[j * stride + 3 * i + 1] - 0.0813 * inputImg[j * stride + 3 * i + 0];
			Y1[j * width + i] = (unsigned char)(Y > 255 ? 255 : (Y < 0 ? 0 : Y));
			//I = (unsigned char)((inputImg[j * stride + 3 * i + 2] + inputImg[j * stride + 3 * i + 1] + inputImg[j * stride + 3 * i + 0]) / 3);
			Y = Y > 255 ? 255 : (Y < 0 ? 0 : Y);
			*/

			dot[0] = Y3[j * width + i];
			dot[1] = Y3[(j - 2) * width + (i - 2)];
			dot[2] = Y3[(j - 2) * width + i];
			dot[3] = Y3[j * width + (i - 2)];

			for (int k = 0; k < 4; k++) {
				if (k == 0) {
					max = dot[k];
					min = dot[k];
				}
				else {
					if (dot[k] > max) {
						max = dot[k];
					}
					else if (dot[k] < min) {
						min = dot[k];
					}
				}
			}
			//high[i / 2][j / 2] = max;
			//low[i / 2][j / 2] = min;
			dot[1] = (int)Y3[(j - 1) * width + (i - 1)];
			dot[2] = (int)Y3[(j - 1) * width + i];
			dot[3] = (int)Y3[j * width + (i - 1)];

			for (int k = 0; k < 4; k++) {
				if (k == 0) {
					max2 = dot[k];
					min2 = dot[k];
				}
				else {
					if (dot[k] > max2) {
						max2 = dot[k];
					}
					else if (dot[k] < min2) {
						min2 = dot[k];
					}
				}
			}
			dif[(max - min)]++;

			if ((max >= max2) && (min <= min2)) difper[(max - min)]++;


			//tmp = (int)inputImg[j * stride + 3*i];
			//N[tmp] += 1;

			//Y3[j * width2 + i] = inputImg2[j * stride2 + 3 * i];

			//Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];
			//Y2[j * width2 + i] = Y1[(j << ratio) * width + (i << ratio)];
			/*Y2[(2 * j) * width2 + 2 * i] = Y1[j * width + i];
			Y2[(2 * j) * width2 + 2 * i + 1] = Y1[j * width + i];
			Y2[(2 * j + 1) * width2 + 2 * i] = Y1[j * width + i];
			Y2[(2 * j + 1) * width2 + 2 * i + 1] = Y1[j * width + i];*/
			//Y2[j * width2 + i] = Y1[(j >> ratio) * width + (i >> ratio)];


			//Y3[j * width + i] = 255 * sumHist[Y1[j * width + i]] / (width * height);
			//if (i % 2 == 1 && j % 2 == 1) mse += (Y1[j * width + i] - Y3[j * width + i]) * (Y1[j * width + i] - Y3[j * width + i]);
			//mse2 += (Y1[j * width + i] - Y3[j * width + i]) * (Y1[j * width + i] - Y3[j * width + i]);

			/*
			outputImg[j * stride + 3 * i + 0] = (unsigned char)Y3[j * width + i];
			outputImg[j * stride + 3 * i + 1] = (unsigned char)Y3[j * width + i];
			outputImg[j * stride + 3 * i + 2] = (unsigned char)Y3[j * width + i];*/
		}
	}

	tmp = 0;
	for (int i = 0; i < 256; i++) {
		//printf("\n%d: %d(%d)", i, dif[i], difper[i]);
		//tmp += dif[i];
	}

	int sum3 = 0;
	int checksum = 0;
	for (int j = 1; j < height; j += 2) {
		for (int i = 1; i < width; i += 2) {
			if (i == 1 || j == 1) {
				if (i > 1) {
					dot[0] = (int)Y2[(j - 1) * width + (i - 2)];
					dot[1] = (int)Y2[(j - 1) * width + (i - 1)];
					dot[2] = (int)Y2[(j - 1) * width + i];
					dot[3] = (int)Y2[j * width + (i - 1)];
					if (dot[0] == dot[1]) checksum++;
					sum3++;
					if (dot[1] == dot[2]) checksum++;
					sum3++;
					if (dot[1] == dot[3]) checksum++;
					sum3++;
				}
				if (j > 1) {
					dot[0] = (int)Y2[(j - 2) * width + (i - 1)];
					dot[1] = (int)Y2[(j - 1) * width + (i - 1)];
					dot[2] = (int)Y2[(j - 1) * width + i];
					dot[3] = (int)Y2[j * width + (i - 1)];
					if (dot[0] == dot[1]) checksum++;
					sum3++;
					if (dot[1] == dot[2]) checksum++;
					sum3++;
					if (dot[1] == dot[3]) checksum++;
					sum3++;
				}
			}
		}
	}
	printf("N windows %d, checksum %d\n", sum3, checksum);


	//printf("\ntotoal%d\n", tmp);
	/*

	for (int j = 1; j < 256; j++) {
		for (int i = 1; i < 256; i++) {
			tmp = high[i][j] - low[i][j];
			if (tmp < 256) dif[tmp]++;
			else printf("out lange tmp(%d)\n", tmp);
		}
	}*/


	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			//Y3[j * width3 + i] = inputImg2[j * stride3 + 3 * i + 0];
			mse += (Y1[j * width + i] - Y4[j * width + i]) * (Y1[j * width + i] - Y4[j * width + i]);
			//mse += (double)((Y1[j * width2 + i] - Y4[j * width3 + i]) * (Y2[j * width2 + i] - Y3[j * width3 + i]));

			outputImg[j * stride + 3 * i + 0] = (unsigned char)Y4[j * width + i];
			outputImg[j * stride + 3 * i + 1] = (unsigned char)Y4[j * width + i];
			outputImg[j * stride + 3 * i + 2] = (unsigned char)Y4[j * width + i];
			//if (i < 9 && j < 9) printf(" %.f", (double)Y2[j * width2 + i]);
		}
	}


	//순서는BGR

	mse /= (width * height);
	psnr = mse != 0.0 ? 10.0 * log10(255 * 255 / mse) : 99.99;
	printf("MSE = %.2lf\nPSNR = %.2lf dB\n", mse, psnr);

	/*
	mse2 /= (width * height);
	psnr2 = mse2 != 0.0 ? 10.0 * log10(255 * 255 / mse2) : 99.99;
	printf("MSE = %.2lf\nPSNR = %.2lf dB\n", mse2, psnr2);*/

	//printf("\ntatal=%d\ncountd=%d", all, countd);

	/*

	for (int j = 1; j < 256; j++) {
		for (int i = 1; i < 256; i++) {
			all++;
			if (high[i][j] <= high2[i][j] && low[i][j] >= low2[i][j]) countd++;
		}
	}
	printf("\ntatal=%d\ncountd=%d", all, countd);
	*/

	/*
	FILE* outputFile = fopen("bitstream.txt", "wb");
	for (int i = 0; i < 256; i++) {
		fprintf(outputFile, "%d: %d\n", i, N[i]);
	}*/


	FILE* outputFile2 = fopen("Output.bmp", "wb");
	//bmpInfo.biWidth = width2;
	//bmpInfo.biHeight = height2;
	//bmpInfo.biSizeImage = size2;
	//bmpFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size2;
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile2);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile2);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile2);

	free(outputImg);
	fclose(outputFile);

	free(inputImg);
	//free(inputImg2);
	fclose(inputFile);
	//fclose(inputFile2);
	fclose(outputFile);
	fclose(outputFile2);

	free(Y1);
	free(Y2);
	free(Y3);
	free(Y4);
	printf("\ncode end\n");
	return 0;
}