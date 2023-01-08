#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<math.h>
//�й�:17011784 �̸�:���ֹ�
//mean ���� �Լ��κ�
void mean(unsigned char* img, int x, int y, int stride, int mode) {		//���ڴ� �̹���, x, y ��ǥ, stride���� ��尪
	unsigned char	Array[9], tmp = 0, mid = 0;							//x,y�� �ֺ��� 3x3�� Ȯ���ϱ� ���� �迭Array[9]���� 
	double sum = 0;														//Y(0~255)�� ������ ���ϱ����� double������ sum ����
	for (int j = x - 1; j <= x + 1; j++) {								//for������ ���� j �� (x-1, x, x+1) ���� ������ ����
		for (int i = y - 1; i <= y + 1; i++) {							//for������ ���� i �� (y-1, y, y+1) ���� ������ ����
			if ((j >= 0 && j <= 511) && (i >= 0 && i <= 511)) {			//��ǥ�� �ǹ��ϴ� i�� j�� ���� [0, 511] ���� �������� �۵��ϴ� ���ǹ�
				Array[(int)tmp++] = img[j * stride + 3 * i];			//Array�迭�� �̹�����(Y)�� �����ϰ� tmp++�� ������ Ƚ���� �� �� �ִ�.
			}
		}
	}																	//�ʿ�������� ������ ������ �ٸ� ����� �Ҷ� ��κ� (int), (unsigned char)�� �ٿ� �����

	if ((int)tmp < 9) {													//���ǹ�: ���� tmp�� 9���� �۴ٸ�? �̴� �ش� ��ǥ��  ������ �Ѿ ��
		sum = 0;														//�Ѿ ������ ó���ϱ� ���� �������� �ϴ� sum=0�Ҵ�
		for (int i = 0; i < (int)tmp; i++) {							//for�� i < tmp�� Array�迭�� �������ִ� ���� �� ��ŭ �۵�
			sum += Array[i];											//Array[i]�� ������ sum�� ���� �����ش�
		}
		mid = (unsigned char)(sum / (int)tmp);							// sum/tmp�� ����� ���ϰ� �װ��� mid ���� �Ҵ�
		for (int i = (int)tmp; i < 9; i++) {							//i=tmp ���� i<9 ���� �������� Array[9] �迭�� �� ������ŭ ����
			Array[(int)tmp++] = mid;									//Array�� �� ������ ������ ���� mid �����η� �Ҵ�
		}
	}

	sum = 0;															//sum ���������� sum=0 �ٽü���

																		//��� ���� ������ ����Ʈ�� ����
	if (mode == 1) {													//��� Ȯ�κκ� ��尪�� 1�� ���� �������� mean���͸� ����
		for (int i = 0; i < 9; i++) {									//for������ [0:9]���� ����
			sum += Array[i];											//sum�� ������ Array[i] ������ ����
		}
		mid = (unsigned char)(sum / 9);									//mid�� ������ ��հ��� sum/9 ���� ����
	}

	else if (mode == 2) {												//��� ���� 2�϶��� ������ mean ���� ���
		for (int i = 0; i < 9; i++) {									//Array[9]�� �����ϱ� ���� for�� ����
			for (int j = 0; j < 9 - i - 1; j++) {						//���Ĺ���� �������ķ� �����ϱ����� for���� �ϳ��� ����
				if (Array[j] > Array[j + 1]) {							//������������ �����ҷ��� ���ǹ�
					tmp = Array[j];										//������ ����� �迭�� �� ������ tmp ������ ���� �� Array[j] ����
					Array[j] = Array[j + 1];							//Array��[j] �� [j+1]���� ����
					Array[j + 1] = tmp;									//[j+1]���� �����ص� tmp�� ���� ����
				}
			}
		}																//for���� �� ������ Array�迭�� ������������ ������ �Ϸ�

		for (int i = 1; i < 8; i++) {									//for������ [1:8]���� ����, ������ �߱⶧���� ���� �ּڰ��� �ִ밪�� ���� ����
			sum += Array[i];											//�ּҰ��� �ִ밪�� �� ������ ������ ���ؼ� sum�� ����
		}
		mid = (unsigned char)(sum / 7);									//mid�� ������ ��հ��� sum/7 ���� ����
	}

	img[x * stride + 3 * y + 0] = mid;									//���� �̹����� ���� mid���� �����ϸ鼭 �Լ�����
	img[x * stride + 3 * y + 1] = mid;									//RGB ���������� YCbCr�� ǥ���ϱ����� �߰��� mid����
	img[x * stride + 3 * y + 2] = mid;									//�Ȱ��� mid�� ����
}

//main�Լ� �κ�
int main() {
	BITMAPFILEHEADER bmpFile;											//������ �о�������� ����
	BITMAPINFOHEADER bmpInfo;											//������ ���ǳ���� ����

	//����1
	FILE* inputFile = NULL;												//FILE������ inputFile ����
	inputFile = fopen("Ou3.bmp", "rb");
	//inputFile = fopen("AICenterY_CombinedNoise.bmp", "rb");				//inputFile�� bmp���� fopen
	if (inputFile == NULL) {											//���Ͼ����� ����
		printf("no file1");												//����Ʈ no file1
		return 0;
	}
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);			//�ش��б�1
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);			//�ش��б�2

	int width = bmpInfo.biWidth;										//width������
	int height = bmpInfo.biHeight;										//height������
	int size = bmpInfo.biSizeImage;										//size������
	int bitCnt = bmpInfo.biBitCount;									//bitCnt������
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;					//Stride������
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n", width, stride, height, size, bitCnt);	//����� ���� Ȯ�ο�

	unsigned char* inputImg = NULL;										//�̹����� �б����� inputImg����
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));		//size��ŭ �����Ҵ�
	fread(inputImg, sizeof(unsigned char), size, inputFile);			//size��ŭ inputImg�� �� ����

	unsigned char* outputImg = NULL;									//�̹��� ������ ���� outputImg����
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));	//size��ŭ �����Ҵ�

	
	FILE* inputFile2 = NULL;											//MSE�� PSNR�� �˱����Ѻκ�
	inputFile2 = fopen("O3.bmp", "rb");						//���� �Է��� �ϳ��� ���ѵǾ� ���� �ּ�ó��
	if (inputFile2 == NULL) {											//�ּ� �ݱ�� ���⸸ ���� �����ϸ� �ٷν��� ����
		printf("no file2");
		return 0;
	}
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile2);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile2);
	unsigned char* inputImg2 = NULL;
	inputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg2, sizeof(unsigned char), size, inputFile2);

	for (int j = 0; j < height; j++) {									//for j [0~511]�����ݺ�
		for (int i = 0; i < width; i++) {								//for i [0~511]�����ݺ� �� 512*512 �� ����

			if (((i >= 1) && (i <= 510)) && ((j >= 1) && (j <= 510))) {	//�̹����� �� ������ ���� ����
				mean(inputImg, j, i, stride, 2);						//��ǥ�� j, i �� �������� �ƴϸ� ��� 2 ����
			}

			else {
				mean(inputImg, j, i, stride, 1);						//��ǥ�� j, i �� �� �����̸� ��� 1 ����
			}

			outputImg[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 0];	//�Լ�median���� �ٲ�
			outputImg[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1];	//inputImg���� ����
			outputImg[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 2];	//outputImg���� ����
		}
	}

	
	double mse = 0, psnr, * Y1, * Y2;									//MSE�� PSNR�� �˱����Ѻκ�2
	Y1 = (double*)calloc(width * height, sizeof(double));				//���� �Է��� �ϳ��� ���ѵǾ� ���� �ּ�ó��
	Y2 = (double*)calloc(width * height, sizeof(double));				//�ּ� �ݱ�� ���⸸ ���� �����ϸ� �ٷν��� ����
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y1[j * width + i] = outputImg[j * stride + 3 * i];
			Y2[j * width + i] = inputImg2[j * stride + 3 * i];
			mse += (double)((Y2[j * width + i] - Y1[j * width + i]) * (Y2[j * width + i] - Y1[j * width + i]));
		}
	}
	mse /= (width * height);
	psnr = mse != 0.0 ? 10.0 * log10(255 * 255 / mse) : 99.99;
	printf("\nMSE = %.2lf\nPSNR = %.2lf dB\n", mse, psnr);
	free(inputImg2);
	fclose(inputFile2);
	free(Y1); free(Y2);
	

	FILE* outputFile = fopen("17011784_���ֹ�.bmp", "wb");				//outFile�� "wb"���� fopen
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);			//�ش����̱�1
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);			//�ش����̱�2
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);			//outputImg�����͸� outFile�� ���̱�
	free(outputImg);													//�����Ҵ� Ǯ��
	fclose(outputFile);													//���ϴݱ�

	free(inputImg);														//�����Ҵ� Ǯ��
	fclose(inputFile);													//���ϴݱ�

	return 0;
}