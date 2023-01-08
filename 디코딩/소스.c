#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include <math.h>

int decoding(char* deco, int mode) {														//���� ���̺�
	if (mode == 1) {																		//���1=4��Ʈ��, ���2=3��Ʈ��
		if (strcmp(deco, "0000") == 0) return 43;											//0000=43
		else if (strcmp(deco, "0001") == 0) return 63;										//0001=63
		else if (strcmp(deco, "0010") == 0) return 81;										//0010=81
		else if (strcmp(deco, "0011") == 0) return 92;										//0011=92
		else if (strcmp(deco, "0100") == 0) return 100;										//0100=100
		else if (strcmp(deco, "0101") == 0) return 109;										//0101=109
		else if (strcmp(deco, "0110") == 0) return 119;										//0110=119
		else if (strcmp(deco, "0111") == 0) return 130;										//0111=130
		else if (strcmp(deco, "1000") == 0) return 143;										//1000=143
		else if (strcmp(deco, "1001") == 0) return 157;										//1001=157
		else if (strcmp(deco, "1010") == 0) return 170;										//1010=170
		else if (strcmp(deco, "1011") == 0) return 181;										//1011=181
		else if (strcmp(deco, "1100") == 0) return 191;										//1100=191
		else if (strcmp(deco, "1101") == 0) return 202;										//1101=202
		else if (strcmp(deco, "1110") == 0) return 215;										//1110=215
		else if (strcmp(deco, "1111") == 0) return 249;										//1111=249
	}
	else if (mode == 2) {																	//���2= 3��Ʈ��
		if (strcmp(deco, "000") == 0) return 57;											//000=57
		else if (strcmp(deco, "001") == 0) return 82;										//000=82
		else if (strcmp(deco, "010") == 0) return 103;										//000=103
		else if (strcmp(deco, "011") == 0) return 125;										//000=125
		else if (strcmp(deco, "100") == 0) return 149;										//000=149
		else if (strcmp(deco, "101") == 0) return 170;										//000=170
		else if (strcmp(deco, "110") == 0) return 192;										//000=192
		else if (strcmp(deco, "111") == 0) return 219;										//000=219
	}
}

int main() {
	BITMAPFILEHEADER bmpFile;																//�����������1
	BITMAPINFOHEADER bmpInfo;																//�����������2

	//����1
	FILE* inputFile = NULL;																	//���������¿뵵
	inputFile = fopen("AICenterY_Org.bmp", "rb");											//512x512bmp���� ����
	if (inputFile == NULL) {
		printf("no file1");																	//���Ͼ����� äũ
		return 0;

	}
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);								//�ش��б�1
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);								//����б�2

	int width = 512;																		//����=512
	int height = 512;																		//����=512
	int size = 786432;																		//������=����x����
	int stride = 1536;																		//��Ʈ���̵�=1536
	printf("W: %d(%d)\nH: %d\nS: %d\n", width, stride, height, size);						//�� äũ

	unsigned char* outputImg = NULL;														//�ƿ�ǲ�̹��� �Ҵ�

	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));						//�����Ҵ����� ũ�����ϱ�


	FILE* inputFile2 = fopen("17011784_���ֹ�.txt", "rb");									//��Ʈ��Ʈ�� �б�
	if (inputFile2 == NULL) {
		printf("no file2");																	//���� ������ äũ
		return 0;

	}
	double* Y1 = (double*)calloc(width * height, sizeof(double));							//������ũ�⸸ŭ �����Ҵ�

	int dot[4] = { 0 }, high = 0, low = 0, tmp = 0;;										//��Ʈ4�� Ȯ�ο��, �ְ�, �ּڰ�Ȯ�ο�
	char bit1, bit3[4] = { 0 }, bit4[5] = { 0 };											//��Ʈ1��, 3��, 4�� Ȯ�ο�
	int mean = 0, b = 0, c = 0;																//�߰��� ����� ���� mean�� (high-low) ���� ���� b, c �������ϱ��

	for (int j = 1; j < height; j += 2) {													//j�� 1���� ���� �� 2������
		for (int i = 1; i < width; i += 2) {												//i�� 1���� ���� �� 2������
			if (i == 1 || j == 1) {															//���� x ��ǥ�� 1�̰ų� y��ǥ�� 1�̸�
				fread(bit4, sizeof(char), 4, inputFile2);									//4��Ʈ�� ��Ʈ��Ʈ������ �б�
				dot[0] = decoding(bit4, 1);													//dot[0]�� ���ΰ�
				fread(bit3, sizeof(char), 3, inputFile2);									//3��Ʈ�� ��Ʈ��Ʈ������ �б�
				dot[1] = decoding(bit3, 2);													//dot[1]�� ���ΰ�
				fread(bit3, sizeof(char), 3, inputFile2);									//3��Ʈ�� ��Ʈ��Ʈ������ �б�
				dot[2] = decoding(bit3, 2);													//dot[2]�� ���ΰ�
				fread(bit3, sizeof(char), 3, inputFile2);									//3��Ʈ�� ��Ʈ��Ʈ������ �б�
				dot[3] = decoding(bit3, 2);													//dot[3]�� ���ΰ�

				Y1[j * width + i] = (double)dot[0];											//Y(i,j)= dot[0] 
				Y1[(j - 1) * width + (i - 1)] = (double)dot[1];								//Y(i-1,j-1)= dot[1] 
				Y1[(j - 1) * width + i] = (double)dot[2];									//Y(i,j-1)= dot[2] 
				Y1[j * width + (i - 1)] = (double)dot[3];									//Y(i-1,j)= dot[3] 
			}
			else {																			//���� x ��ǥ�� 1�� �ƴϰ� y��ǥ�� 1�� �ƴϸ�
				dot[1] = (int)Y1[(j - 2) * width + (i - 2)];								//dot[1]=Y(i-2,j-2)
				dot[2] = (int)Y1[(j - 2) * width + i];										//dot[2]=Y(i,j-2)
				dot[3] = (int)Y1[j * width + (i - 2)];										//dot[3]=Y(i-2,j) �������� Y������ �̹� ���ڵ��ؼ� �˼��ִ� ����

				fread(&bit1, sizeof(char), 1, inputFile2);									//1��Ʈ�� ��Ʈ��Ʈ������ �б�
				if (bit1 == '0') {															//��Ʈ�� 0�̸� Y(i,j)�� ���� dot[2] or dot[3]�� �������� �ִٴ� ��
					fread(&bit1, sizeof(char), 1, inputFile2);								//1��Ʈ�� ��Ʈ��Ʈ������ �б�
					if (bit1 == '0') {														//��Ʈ�� 0�̸�
						dot[0] = dot[2];													//dot[2]�� Y(i,j)=dot[0]�� ���ٴ°�
					}
					else {																	//��Ʈ�� 1�̸�
						dot[0] = dot[3];													//dot[3]�� Y(i,j)=dot[0]�� ���ٴ°�
					}																		//���� ����� ����Y(i,j)�� 4��Ʈ �����͸� 2��Ʈ�� ���ϼ� �־���
				}																			//��� �ߺ����� �������� Ȯ�ο� 1��Ʈ�� ���
				else {																		//��Ʈ�� 1�̿��ٸ�
					fread(bit4, sizeof(char), 4, inputFile2);								//4��Ʈ�� ��Ʈ��Ʈ������ �б�
					dot[0] = decoding(bit4, 1);												//dot[0]�� ���ΰ�
				}																			//�� �ڵ�κп��� dot[0]�� ���� ������

				high = dot[0];																//dot[0~4]�� �ּҰ� �ִ밪�� ���ϴ� ����
				low = dot[0];
				for (int k = 1; k < 4; k++) {
					if (dot[k] > high) {
						high = dot[k];
					}
					else if (dot[k] < low) {
						low = dot[k];
					}
				}																			//������ high ���� �ִ밪 low ���� �ּڰ�

				if ((high - low) == 0) {													//�ִ밪-�ּҰ� = 0 �̸�
					fread(&bit1, sizeof(char), 1, inputFile2);								//1��Ʈ�� �б�
					if (bit1 == '0') {														//��Ʈ�� 0�̸� ������ ���� Y������ ��� ���� ����� �ǹ�
						Y1[j * width + i] = (double)(dot[0]);								//�� ���������̹Ƿ� Y(i,j)=dot[0]
						Y1[(j - 1) * width + (i - 1)] = (double)(dot[0]);					//Y(i-1,j-1)=dot[1]
						Y1[(j - 1) * width + i] = (double)(dot[0]);							//Y(i,j-1)=dot[2]
						Y1[j * width + (i - 1)] = (double)(dot[0]);							//Y(i-1,j)=dot[3], �� ��� �ſ����� ��Ʈ�� ������1���� ��Ұ����� ������
					}
					else {																	//1��Ʈ �о��µ� 1�̸� ������ ���� ��������� �ִ밪 �ּڰ��� ������ ��� �ǹ�
						fread(bit3, sizeof(char), 3, inputFile2);							//�̰�� 3��Ʈ �����͸� �׳� �о ó��, 3��Ʈ �б�
						dot[1] = decoding(bit3, 2);											//dot[1]�� 3��Ʈ ����
						fread(bit3, sizeof(char), 3, inputFile2);							//3��Ʈ �б�
						dot[2] = decoding(bit3, 2);											//dot[2]�� 3��Ʈ ����
						fread(bit3, sizeof(char), 3, inputFile2);							//3��Ʈ �б�
						dot[3] = decoding(bit3, 2);											//dot[3]�� 3��Ʈ ����

						Y1[j * width + i] = (double)(dot[0]);								//Y(i,j)=dot[0]			���� �κ��� �ߺ����� ��� ���ɴϴ�
						Y1[(j - 1) * width + (i - 1)] = (double)(dot[1]);					//Y(i-1,j-1)=dot[1]
						Y1[(j - 1) * width + i] = (double)(dot[2]);							//Y(i,j-1)=dot[2]
						Y1[j * width + (i - 1)] = (double)(dot[3]);							//Y(i-1,j)=dot[3]
					}
				}
				else if ((high - low) <= 14) {												//�ִ밪-�ּҰ� <= 14 �̸�
					fread(&bit1, sizeof(char), 1, inputFile2);								//1��Ʈ�� �б�
					if (bit1 == '0') {														//��Ʈ�� 0�̸� ������ ���� Y������ ��� ���� ����� �ǹ�
						Y1[j * width + i] = dot[0];											//Y(i,j)=dot[0] �� �Է�

						mean = (high + low) / 2;											//mean�� �ִ밪 �ּҰ� �߰���

						fread(&bit1, sizeof(char), 1, inputFile2);							//1��Ʈ �б�
						if (bit1 == '0') Y1[(j - 1) * width + (i - 1)] = (double)(mean - 4);//1��Ʈ�� 0�̸�, Y(i-1,j-1)=mean-4
						else Y1[(j - 1) * width + (i - 1)] = (double)(mean + 4);			//1��Ʈ�� 1�̸�, Y(i-1,j-1)=mean+4

						fread(&bit1, sizeof(char), 1, inputFile2);							//1��Ʈ �б�
						if (bit1 == '0') Y1[(j - 1) * width + i] = (double)(mean - 4);		//1��Ʈ�� 0�̸�, Y(i,j-1)=mean-4
						else Y1[(j - 1) * width + i] = (double)(mean + 4);					//1��Ʈ�� 1�̸�, Y(i,j-1)=mean+4

						fread(&bit1, sizeof(char), 1, inputFile2);							//1��Ʈ �б�
						if (bit1 == '0') Y1[j * width + (i - 1)] = (double)(mean - 4);		//1��Ʈ�� 0�̸�, Y(i-1,j)=mean+4
						else Y1[j * width + (i - 1)] = (double)(mean + 4);					//1��Ʈ�� 1�̸�, Y(i-1,j)=mean+4
					}
					else {																	//���� ��Ʈ�� 1�϶� ������ ����� ��Ʈ�� ����
						fread(bit3, sizeof(char), 3, inputFile2);							//�̰�� 3��Ʈ �����͸� �׳� �о ó��, 3��Ʈ �б�
						dot[1] = decoding(bit3, 2);											//dot[1]�� 3��Ʈ ����
						fread(bit3, sizeof(char), 3, inputFile2);							//3��Ʈ �б�
						dot[2] = decoding(bit3, 2);											//dot[2]�� 3��Ʈ ����
						fread(bit3, sizeof(char), 3, inputFile2);							//3��Ʈ �б�
						dot[3] = decoding(bit3, 2);											//dot[3]�� 3��Ʈ ����

						Y1[j * width + i] = (double)(dot[0]);								//Y(i,j)=dot[0]
						Y1[(j - 1) * width + (i - 1)] = (double)(dot[1]);					//Y(i-1,j-1)=dot[1]
						Y1[(j - 1) * width + i] = (double)(dot[2]);							//Y(i,j-1)=dot[2]
						Y1[j * width + (i - 1)] = (double)(dot[3]);							//Y(i-1,j)=dot[3]
					}
				}

				else if ((high - low) <= 38) {												//14 < �ִ밪-�ּҰ� <= 38 �̸�
					fread(&bit1, sizeof(char), 1, inputFile2);								//��Ʈ 1�� �б�
					if (bit1 == '0') {														//��Ʈ�� 0 �̸� ������ ������ ���� ���� ����
						Y1[j * width + i] = dot[0];											//Y(i,j)=dot[0]

						mean = (high + low) / 2;											//mean�� �ְ� �ּҰ��� ��հ�
						if ((high - low) <= 30) { b = 4; c = 13; }							//14 < �ִ밪-�ּҰ� <= 30 �̸� b=4 c=13
						else if ((high - low) <= 38) { b = 5; c = 17; }						//10 < �ִ밪-�ּҰ� <= 38 �̸� b=5 c=17

						fread(&bit1, sizeof(char), 1, inputFile2);							//1��Ʈ �б� Y(i-1,j-1)�κ�
						if (bit1 == '0') {													//0�̸� ��հ����� ���̳ʽ�
							fread(&bit1, sizeof(char), 1, inputFile2);						//1��Ʈ�б� 
							if (bit1 == '0') Y1[(j - 1) * width + (i - 1)] = (double)(mean - c);//��Ʈ�� 0 �̸� Y(i-1,j-1)=mean-c
							else Y1[(j - 1) * width + (i - 1)] = (double)(mean - b);		//��Ʈ�� 1 �̸� Y(i-1,j-1)=mean-b
						}
						else {																//���� ��Ʈ�� 1�̸� ��հ� ���� �÷���
							fread(&bit1, sizeof(char), 1, inputFile2);						//1��Ʈ �б�
							if (bit1 == '0') Y1[(j - 1) * width + (i - 1)] = (double)(mean + b);//��Ʈ�� 0 �̸�Y(i-1,j-1)=mean+b
							else Y1[(j - 1) * width + (i - 1)] = (double)(mean + c);		//��Ʈ�� 1 �̸�Y(i-1,j-1)=mean+c
						}

						fread(&bit1, sizeof(char), 1, inputFile2);							//1��Ʈ �б� Y(i,j-1)�κ�
						if (bit1 == '0') {													//0�̸� ��հ����� ���̳ʽ�
							fread(&bit1, sizeof(char), 1, inputFile2);						//1��Ʈ�б� 
							if (bit1 == '0') Y1[(j - 1) * width + i] = (double)(mean - c);	//��Ʈ�� 0 �̸� Y(i,j-1)=mean-c
							else Y1[(j - 1) * width + i] = (double)(mean - b);				//��Ʈ�� 1 �̸� Y(i,j-1)=mean-b
						}
						else {																//���� ��Ʈ�� 1�̸� ��հ� ���� �÷���
							fread(&bit1, sizeof(char), 1, inputFile2);						//1��Ʈ �б�
							if (bit1 == '0') Y1[(j - 1) * width + i] = (double)(mean + b);	//��Ʈ�� 0 �̸�Y(i,j-1)=mean+b
							else Y1[(j - 1) * width + i] = (double)(mean + c);				//��Ʈ�� 1 �̸�Y(i,j-1)=mean+c
						}

						fread(&bit1, sizeof(char), 1, inputFile2);							//1��Ʈ �б� Y(i-1,j)�κ�
						if (bit1 == '0') {													//0�̸� ��հ����� ���̳ʽ�
							fread(&bit1, sizeof(char), 1, inputFile2);						//1��Ʈ�б�
							if (bit1 == '0') Y1[j * width + (i - 1)] = (double)(mean - c);	//��Ʈ�� 0 �̸� Y(i-1,j)=mean-c
							else Y1[j * width + (i - 1)] = (double)(mean - b);				//��Ʈ�� 1 �̸� Y(i-1,j)=mean-b
						}
						else {																//���� ��Ʈ�� 1�̸� ��հ� ���� �÷���
							fread(&bit1, sizeof(char), 1, inputFile2);						//1��Ʈ �б�	
							if (bit1 == '0') Y1[j * width + (i - 1)] = (double)(mean + b);	//��Ʈ�� 0 �̸�Y(i-1,j)=mean+b
							else Y1[j * width + (i - 1)] = (double)(mean + c);				//��Ʈ�� 1 �̸�Y(i-1,j)=mean+c
						}
					}
					else {																	//������Ʈ�� 1�Ͻ� �������� �� ���� �̰�� 3��Ʈ�� ó��
						fread(bit3, sizeof(char), 3, inputFile2);							//��Ʈ3 �б�
						dot[1] = decoding(bit3, 2);											//dot[1]�� 3��Ʈ ����
						fread(bit3, sizeof(char), 3, inputFile2);							//��Ʈ3 �б�
						dot[2] = decoding(bit3, 2);											//dot[2]�� 3��Ʈ ����
						fread(bit3, sizeof(char), 3, inputFile2);							//��Ʈ3 �б�
						dot[3] = decoding(bit3, 2);											//dot[3]�� 3��Ʈ ����

						Y1[j * width + i] = (double)(dot[0]);								//Y(i,j)=dot[0]
						Y1[(j - 1) * width + (i - 1)] = (double)(dot[1]);					//Y(i-1,j-1)=dot[1]
						Y1[(j - 1) * width + i] = (double)(dot[2]);							//Y(i,j-1)=dot[2]
						Y1[j * width + (i - 1)] = (double)(dot[3]);							//Y(i-1,j)=dot[3]
					}
				}
				else {																		//�ּҰ� �ִ밪�� 38�� �Ѿ�°�� Ȯ�κ�Ʈ ���� 3��Ʈ�� ó��
					fread(bit3, sizeof(char), 3, inputFile2);								//��Ʈ3 �б�
					dot[1] = decoding(bit3, 2);												//dot[1]�� 3��Ʈ ����
					fread(bit3, sizeof(char), 3, inputFile2);								//��Ʈ3 �б�
					dot[2] = decoding(bit3, 2);												//dot[2]�� 3��Ʈ ����
					fread(bit3, sizeof(char), 3, inputFile2);								//��Ʈ3 �б�
					dot[3] = decoding(bit3, 2);												//dot[3]�� 3��Ʈ ����

					Y1[j * width + i] = (double)(dot[0]);									//Y(i,j)=dot[0]
					Y1[(j - 1) * width + (i - 1)] = (double)(dot[1]);						//Y(i-1,j-1)=dot[1]
					Y1[(j - 1) * width + i] = (double)(dot[2]);								//Y(i,j-1)=dot[2]
					Y1[j * width + (i - 1)] = (double)(dot[3]);								//Y(i-1,j)=dot[3]
				}
			}
		}
	}

	double Y, mse = 0, psnr, mse2 = 0, psnr2;


	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			//mse += (Y1[j * width + i] - Y4[j * width + i]) * (Y1[j * width + i] - Y4[j * width + i]);
			outputImg[j * stride + 3 * i + 0] = (unsigned char)Y1[j * width + i];			//outputimg ���� Y1��
			outputImg[j * stride + 3 * i + 1] = (unsigned char)Y1[j * width + i];			//outputimg ���� Y1��
			outputImg[j * stride + 3 * i + 2] = (unsigned char)Y1[j * width + i];			//outputimg ���� Y1��
		}
	}

	/*																						//psnr
	mse /= (width * height);
	psnr = mse != 0.0 ? 10.0 * log10(255 * 255 / mse) : 99.99;
	printf("MSE = %.2lf\nPSNR = %.2lf dB\n", mse, psnr);*/


	FILE* outputFile = fopen("17011784_���ֹ�.bmp", "wb");									//bmp ���
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);								//�ش� ���̱�1
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);								//�ش� ���̱�2
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);								//outputimg�� ���̱�

	free(outputImg);																		

	fclose(inputFile);
	fclose(outputFile);

	free(Y1);

	printf("\ncode end\n");																	//�ڵ� ���� Ȯ��
	return 0;
}