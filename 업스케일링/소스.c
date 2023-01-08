#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<math.h>

/*
memo
18.85
19.28
*/

double upScale(int x, int ratio, double* y) {
	double a = y[0], b = y[1];
	double dx1 = ((double)x / (double)ratio), dx2 = 1 - dx1, up;
	up = a * dx2 + b * dx1;
	return up;
}

int main() {
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;

	FILE* inputFile1 = NULL;
	inputFile1 = fopen("17011784.bmp", "rb");
	if (inputFile1 == NULL) {
		printf("no file 1");
		return 0;
	}
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile1);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile1);

	int width2 = bmpInfo.biWidth;
	int height2 = bmpInfo.biHeight;
	int size2 = bmpInfo.biSizeImage;
	int stride2 = width2 * 3;

	int width = width2 * 4;
	int height = height2 * 4;							//height = width2 * 4;���� ���� �̺κ��� width2==height2�� �������
	int stride = width * 3;								//stride = width2 * 3;���� ���� ���������� ������� �κ�
	int size = stride * height;							//size = stride2 * height2; ���� ���� stride2,height2 -> stride,heightȥ��

	

	unsigned char* inputImg = (unsigned char*)calloc(size2, sizeof(unsigned char));	//����size->size2 ũ��ȥ��
	unsigned char* outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

	fread(inputImg, sizeof(unsigned char), size2, inputFile1);						//����size->size2 ũ��ȥ��
	
	double* Y1 = (double*)calloc(width2 * height2, sizeof(double));					//����width*height->width2*height2, �����δ� Y1 ����� ��� �������
	double* Y2 = (double*)calloc(width * height, sizeof(double));
	double* Y4 = (double*)calloc(width2 * height2, sizeof(double));
	double* Y5 = (double*)calloc(width * height, sizeof(double));
	double* Y6 = (double*)calloc(width * height, sizeof(double));

	double Y = 0, mse = 0, psnr, z[2] = { 0 };
	for (int j = 0; j < height2; j++) {												//����height->height2
		for (int i = 0; i < width2; i++) {											//����width->width2
			Y4[j * width2 + i] = (double)inputImg[j * stride2 + 3 * i];				//����width, stride->width2, stride2
		}
	}
	
	for (int j = 0; j < height2; j++) {
		for (int i = 0; i < width2; i++) {
			for (int k = 0; k < 2; k++) {
				if ((i + k) < width2) {
					z[k] = Y4[j * width2 + i + k];
				}
				else z[k] = z[k - 1];
			}
			for (int k = 0; k < 4; k++) {
				Y = upScale(k, 4, z);
				Y5[j * width + 4 * i + k] = Y;
			}
		}
	}
	
	for (int j = 0; j < height2; j++) {
		for (int i = 0; i < width; i++) {
			for (int k = 0; k < 2; k++) {
				if ((j + k) < height2) {
					z[k] = Y5[(j + k) * width + i];
				}
				else z[k] = z[k - 1];
			}
			for (int k = 0; k < 4; k++) {
				Y = upScale(k, 4, z);
				Y6[(j * 4 + k) * width + i] = Y;
			}

		}
	}
	


	/*
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y2[j * width + i] = Y4[(j / 4) * width2 + (i / 4)];
			Y = Y2[j * width + i];
			Y = Y > 255 ? 255 : (Y < 0 ? 0 : Y);


			mse += (double)((Y1[j * width + i] - Y) * (Y1[j * width + i] - Y));
		}
	}*/

	

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y = Y6[j * width + i];
			Y = Y > 255 ? 255 : (Y < 0 ? 0 : Y);

			//mse += (double)((Y1[j * width + i] - Y) * (Y1[j * width + i] - Y));			//�������� �ּ� ó�� ����

			outputImg[j * stride + 3 * i + 0] = (unsigned char)Y;
			outputImg[j * stride + 3 * i + 1] = (unsigned char)Y;
			outputImg[j * stride + 3 * i + 2] = (unsigned char)Y;
		}
	}
	printf("check point4");

	/*
	mse /= (width * height);																//�������� �ּ� ó�� ����
	psnr = mse != 0.0 ? 10.0 * log10(255 * 255 / mse) : 99.99;								//�������� �ּ� ó�� ����
	printf("MSE = %.2lf\nPSNR = %.2lf dB\n", mse, psnr);*/									//�������� �ּ� ó�� ����

	FILE* outputFile = fopen("Output.bmp", "wb");
	bmpInfo.biWidth = width;																//�������� �ּ��̿��� width2->width
	bmpInfo.biHeight = height;																//�������� �ּ��̿��� height2->height
	bmpInfo.biSizeImage = size;																//�������� �ּ��̿��� size2->size
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);
	printf("check point5");
	free(Y1);
	free(Y2);
	free(Y4);
	free(Y5);
	free(Y6);
	free(outputImg);
	free(inputImg);
	fclose(inputFile1);
	fclose(outputFile);
	printf("\ncode end\n");
	return 0;
}