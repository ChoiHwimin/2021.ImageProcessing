#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include <math.h>

int decoding(char* deco, int mode) {														//매핑 테이블
	if (mode == 1) {																		//모드1=4비트용, 모드2=3비트용
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
	else if (mode == 2) {																	//모드2= 3비트용
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
	BITMAPFILEHEADER bmpFile;																//파일헤더선언1
	BITMAPINFOHEADER bmpInfo;																//파일헤더선언2

	//파일1
	FILE* inputFile = NULL;																	//헤더정보얻는용도
	inputFile = fopen("AICenterY_Org.bmp", "rb");											//512x512bmp파일 열기
	if (inputFile == NULL) {
		printf("no file1");																	//파일없을시 채크
		return 0;

	}
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);								//해더읽기1
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);								//헤더읽기2

	int width = 512;																		//가로=512
	int height = 512;																		//세로=512
	int size = 786432;																		//사이즈=가로x세로
	int stride = 1536;																		//스트라이드=1536
	printf("W: %d(%d)\nH: %d\nS: %d\n", width, stride, height, size);						//값 채크

	unsigned char* outputImg = NULL;														//아웃풋이미지 할당

	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));						//동적할당으로 크기정하기


	FILE* inputFile2 = fopen("17011784_최휘민.txt", "rb");									//비트스트림 읽기
	if (inputFile2 == NULL) {
		printf("no file2");																	//파일 없으면 채크
		return 0;

	}
	double* Y1 = (double*)calloc(width * height, sizeof(double));							//사이즈크기만큼 동적할당

	int dot[4] = { 0 }, high = 0, low = 0, tmp = 0;;										//도트4개 확인용과, 최고값, 최솟값확인용
	char bit1, bit3[4] = { 0 }, bit4[5] = { 0 };											//비트1개, 3개, 4개 확인용
	int mean = 0, b = 0, c = 0;																//중간값 계산을 위한 mean과 (high-low) 값에 따른 b, c 영역정하기용

	for (int j = 1; j < height; j += 2) {													//j는 1부터 시작 후 2씩증가
		for (int i = 1; i < width; i += 2) {												//i는 1부터 시작 후 2씩증가
			if (i == 1 || j == 1) {															//만약 x 좌표가 1이거나 y좌표가 1이면
				fread(bit4, sizeof(char), 4, inputFile2);									//4비트를 비트스트림에서 읽기
				dot[0] = decoding(bit4, 1);													//dot[0]에 매핑값
				fread(bit3, sizeof(char), 3, inputFile2);									//3비트를 비트스트림에서 읽기
				dot[1] = decoding(bit3, 2);													//dot[1]에 매핑값
				fread(bit3, sizeof(char), 3, inputFile2);									//3비트를 비트스트림에서 읽기
				dot[2] = decoding(bit3, 2);													//dot[2]에 매핑값
				fread(bit3, sizeof(char), 3, inputFile2);									//3비트를 비트스트림에서 읽기
				dot[3] = decoding(bit3, 2);													//dot[3]에 매핑값

				Y1[j * width + i] = (double)dot[0];											//Y(i,j)= dot[0] 
				Y1[(j - 1) * width + (i - 1)] = (double)dot[1];								//Y(i-1,j-1)= dot[1] 
				Y1[(j - 1) * width + i] = (double)dot[2];									//Y(i,j-1)= dot[2] 
				Y1[j * width + (i - 1)] = (double)dot[3];									//Y(i-1,j)= dot[3] 
			}
			else {																			//만약 x 좌표가 1이 아니고 y좌표가 1이 아니면
				dot[1] = (int)Y1[(j - 2) * width + (i - 2)];								//dot[1]=Y(i-2,j-2)
				dot[2] = (int)Y1[(j - 2) * width + i];										//dot[2]=Y(i,j-2)
				dot[3] = (int)Y1[j * width + (i - 2)];										//dot[3]=Y(i-2,j) 오른쪽의 Y값들은 이미 디코딩해서 알수있는 값들

				fread(&bit1, sizeof(char), 1, inputFile2);									//1비트를 비트스트림에서 읽기
				if (bit1 == '0') {															//비트가 0이면 Y(i,j)의 값이 dot[2] or dot[3]에 같은값이 있다는 것
					fread(&bit1, sizeof(char), 1, inputFile2);								//1비트를 비트스트림에서 읽기
					if (bit1 == '0') {														//비트가 0이면
						dot[0] = dot[2];													//dot[2]와 Y(i,j)=dot[0]이 같다는것
					}
					else {																	//비트가 1이면
						dot[0] = dot[3];													//dot[3]과 Y(i,j)=dot[0]이 같다는것
					}																		//위의 결과로 기존Y(i,j)의 4비트 데이터를 2비트로 줄일수 있었음
				}																			//대신 중복값이 없을때도 확인용 1비트를 사용
				else {																		//비트가 1이였다면
					fread(bit4, sizeof(char), 4, inputFile2);								//4비트를 비트스트림에서 읽기
					dot[0] = decoding(bit4, 1);												//dot[0]에 매핑값
				}																			//위 코드부분에서 dot[0]의 값이 정해짐

				high = dot[0];																//dot[0~4]의 최소값 최대값을 구하는 과정
				low = dot[0];
				for (int k = 1; k < 4; k++) {
					if (dot[k] > high) {
						high = dot[k];
					}
					else if (dot[k] < low) {
						low = dot[k];
					}
				}																			//과정끝 high 값에 최대값 low 값에 최솟값

				if ((high - low) == 0) {													//최대값-최소값 = 0 이면
					fread(&bit1, sizeof(char), 1, inputFile2);								//1비트를 읽기
					if (bit1 == '0') {														//비트가 0이면 윈도우 내의 Y값들이 모두 범위 내라는 의미
						Y1[j * width + i] = (double)(dot[0]);								//다 같은범위이므로 Y(i,j)=dot[0]
						Y1[(j - 1) * width + (i - 1)] = (double)(dot[0]);					//Y(i-1,j-1)=dot[1]
						Y1[(j - 1) * width + i] = (double)(dot[0]);							//Y(i,j-1)=dot[2]
						Y1[j * width + (i - 1)] = (double)(dot[0]);							//Y(i-1,j)=dot[3], 이 경우 매우적은 비트로 윈도우1개의 요소값들이 정해짐
					}
					else {																	//1비트 읽었는데 1이면 윈도우 내의 어느값들은 최대값 최솟값의 범위를 벗어난 의미
						fread(bit3, sizeof(char), 3, inputFile2);							//이경우 3비트 데이터를 그냥 읽어서 처리, 3비트 읽기
						dot[1] = decoding(bit3, 2);											//dot[1]에 3비트 매핑
						fread(bit3, sizeof(char), 3, inputFile2);							//3비트 읽기
						dot[2] = decoding(bit3, 2);											//dot[2]에 3비트 매핑
						fread(bit3, sizeof(char), 3, inputFile2);							//3비트 읽기
						dot[3] = decoding(bit3, 2);											//dot[3]에 3비트 매핑

						Y1[j * width + i] = (double)(dot[0]);								//Y(i,j)=dot[0]			여기 부분은 중복으로 계속 나옵니다
						Y1[(j - 1) * width + (i - 1)] = (double)(dot[1]);					//Y(i-1,j-1)=dot[1]
						Y1[(j - 1) * width + i] = (double)(dot[2]);							//Y(i,j-1)=dot[2]
						Y1[j * width + (i - 1)] = (double)(dot[3]);							//Y(i-1,j)=dot[3]
					}
				}
				else if ((high - low) <= 14) {												//최대값-최소값 <= 14 이면
					fread(&bit1, sizeof(char), 1, inputFile2);								//1비트를 읽기
					if (bit1 == '0') {														//비트가 0이면 윈도우 내의 Y값들이 모두 범위 내라는 의미
						Y1[j * width + i] = dot[0];											//Y(i,j)=dot[0] 로 입력

						mean = (high + low) / 2;											//mean에 최대값 최소값 중간값

						fread(&bit1, sizeof(char), 1, inputFile2);							//1비트 읽기
						if (bit1 == '0') Y1[(j - 1) * width + (i - 1)] = (double)(mean - 4);//1비트가 0이면, Y(i-1,j-1)=mean-4
						else Y1[(j - 1) * width + (i - 1)] = (double)(mean + 4);			//1비트가 1이면, Y(i-1,j-1)=mean+4

						fread(&bit1, sizeof(char), 1, inputFile2);							//1비트 읽기
						if (bit1 == '0') Y1[(j - 1) * width + i] = (double)(mean - 4);		//1비트가 0이면, Y(i,j-1)=mean-4
						else Y1[(j - 1) * width + i] = (double)(mean + 4);					//1비트가 1이면, Y(i,j-1)=mean+4

						fread(&bit1, sizeof(char), 1, inputFile2);							//1비트 읽기
						if (bit1 == '0') Y1[j * width + (i - 1)] = (double)(mean - 4);		//1비트가 0이면, Y(i-1,j)=mean+4
						else Y1[j * width + (i - 1)] = (double)(mean + 4);					//1비트가 1이면, Y(i-1,j)=mean+4
					}
					else {																	//읽은 비트가 1일때 범위를 벗어나는 비트가 존재
						fread(bit3, sizeof(char), 3, inputFile2);							//이경우 3비트 데이터를 그냥 읽어서 처리, 3비트 읽기
						dot[1] = decoding(bit3, 2);											//dot[1]에 3비트 매핑
						fread(bit3, sizeof(char), 3, inputFile2);							//3비트 읽기
						dot[2] = decoding(bit3, 2);											//dot[2]에 3비트 매핑
						fread(bit3, sizeof(char), 3, inputFile2);							//3비트 읽기
						dot[3] = decoding(bit3, 2);											//dot[3]에 3비트 매핑

						Y1[j * width + i] = (double)(dot[0]);								//Y(i,j)=dot[0]
						Y1[(j - 1) * width + (i - 1)] = (double)(dot[1]);					//Y(i-1,j-1)=dot[1]
						Y1[(j - 1) * width + i] = (double)(dot[2]);							//Y(i,j-1)=dot[2]
						Y1[j * width + (i - 1)] = (double)(dot[3]);							//Y(i-1,j)=dot[3]
					}
				}

				else if ((high - low) <= 38) {												//14 < 최대값-최소값 <= 38 이면
					fread(&bit1, sizeof(char), 1, inputFile2);								//비트 1개 읽기
					if (bit1 == '0') {														//비트가 0 이면 윈도우 값들이 범위 내에 존재
						Y1[j * width + i] = dot[0];											//Y(i,j)=dot[0]

						mean = (high + low) / 2;											//mean에 최고값 최소값의 평균값
						if ((high - low) <= 30) { b = 4; c = 13; }							//14 < 최대값-최소값 <= 30 이면 b=4 c=13
						else if ((high - low) <= 38) { b = 5; c = 17; }						//10 < 최대값-최소값 <= 38 이면 b=5 c=17

						fread(&bit1, sizeof(char), 1, inputFile2);							//1비트 읽기 Y(i-1,j-1)부분
						if (bit1 == '0') {													//0이면 평균값보다 마이너스
							fread(&bit1, sizeof(char), 1, inputFile2);						//1비트읽기 
							if (bit1 == '0') Y1[(j - 1) * width + (i - 1)] = (double)(mean - c);//비트가 0 이면 Y(i-1,j-1)=mean-c
							else Y1[(j - 1) * width + (i - 1)] = (double)(mean - b);		//비트가 1 이면 Y(i-1,j-1)=mean-b
						}
						else {																//읽은 비트가 1이면 평균값 보다 플러스
							fread(&bit1, sizeof(char), 1, inputFile2);						//1비트 읽기
							if (bit1 == '0') Y1[(j - 1) * width + (i - 1)] = (double)(mean + b);//비트가 0 이면Y(i-1,j-1)=mean+b
							else Y1[(j - 1) * width + (i - 1)] = (double)(mean + c);		//비트가 1 이면Y(i-1,j-1)=mean+c
						}

						fread(&bit1, sizeof(char), 1, inputFile2);							//1비트 읽기 Y(i,j-1)부분
						if (bit1 == '0') {													//0이면 평균값보다 마이너스
							fread(&bit1, sizeof(char), 1, inputFile2);						//1비트읽기 
							if (bit1 == '0') Y1[(j - 1) * width + i] = (double)(mean - c);	//비트가 0 이면 Y(i,j-1)=mean-c
							else Y1[(j - 1) * width + i] = (double)(mean - b);				//비트가 1 이면 Y(i,j-1)=mean-b
						}
						else {																//읽은 비트가 1이면 평균값 보다 플러스
							fread(&bit1, sizeof(char), 1, inputFile2);						//1비트 읽기
							if (bit1 == '0') Y1[(j - 1) * width + i] = (double)(mean + b);	//비트가 0 이면Y(i,j-1)=mean+b
							else Y1[(j - 1) * width + i] = (double)(mean + c);				//비트가 1 이면Y(i,j-1)=mean+c
						}

						fread(&bit1, sizeof(char), 1, inputFile2);							//1비트 읽기 Y(i-1,j)부분
						if (bit1 == '0') {													//0이면 평균값보다 마이너스
							fread(&bit1, sizeof(char), 1, inputFile2);						//1비트읽기
							if (bit1 == '0') Y1[j * width + (i - 1)] = (double)(mean - c);	//비트가 0 이면 Y(i-1,j)=mean-c
							else Y1[j * width + (i - 1)] = (double)(mean - b);				//비트가 1 이면 Y(i-1,j)=mean-b
						}
						else {																//읽은 비트가 1이면 평균값 보다 플러스
							fread(&bit1, sizeof(char), 1, inputFile2);						//1비트 읽기	
							if (bit1 == '0') Y1[j * width + (i - 1)] = (double)(mean + b);	//비트가 0 이면Y(i-1,j)=mean+b
							else Y1[j * width + (i - 1)] = (double)(mean + c);				//비트가 1 이면Y(i-1,j)=mean+c
						}
					}
					else {																	//읽은비트가 1일시 범위밖의 값 존재 이경우 3비트로 처리
						fread(bit3, sizeof(char), 3, inputFile2);							//비트3 읽기
						dot[1] = decoding(bit3, 2);											//dot[1]에 3비트 매핑
						fread(bit3, sizeof(char), 3, inputFile2);							//비트3 읽기
						dot[2] = decoding(bit3, 2);											//dot[2]에 3비트 매핑
						fread(bit3, sizeof(char), 3, inputFile2);							//비트3 읽기
						dot[3] = decoding(bit3, 2);											//dot[3]에 3비트 매핑

						Y1[j * width + i] = (double)(dot[0]);								//Y(i,j)=dot[0]
						Y1[(j - 1) * width + (i - 1)] = (double)(dot[1]);					//Y(i-1,j-1)=dot[1]
						Y1[(j - 1) * width + i] = (double)(dot[2]);							//Y(i,j-1)=dot[2]
						Y1[j * width + (i - 1)] = (double)(dot[3]);							//Y(i-1,j)=dot[3]
					}
				}
				else {																		//최소값 최대값이 38을 넘어가는경우 확인비트 없이 3비트로 처리
					fread(bit3, sizeof(char), 3, inputFile2);								//비트3 읽기
					dot[1] = decoding(bit3, 2);												//dot[1]에 3비트 매핑
					fread(bit3, sizeof(char), 3, inputFile2);								//비트3 읽기
					dot[2] = decoding(bit3, 2);												//dot[2]에 3비트 매핑
					fread(bit3, sizeof(char), 3, inputFile2);								//비트3 읽기
					dot[3] = decoding(bit3, 2);												//dot[3]에 3비트 매핑

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
			outputImg[j * stride + 3 * i + 0] = (unsigned char)Y1[j * width + i];			//outputimg 값에 Y1값
			outputImg[j * stride + 3 * i + 1] = (unsigned char)Y1[j * width + i];			//outputimg 값에 Y1값
			outputImg[j * stride + 3 * i + 2] = (unsigned char)Y1[j * width + i];			//outputimg 값에 Y1값
		}
	}

	/*																						//psnr
	mse /= (width * height);
	psnr = mse != 0.0 ? 10.0 * log10(255 * 255 / mse) : 99.99;
	printf("MSE = %.2lf\nPSNR = %.2lf dB\n", mse, psnr);*/


	FILE* outputFile = fopen("17011784_최휘민.bmp", "wb");									//bmp 출력
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);								//해더 붙이기1
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);								//해더 붙이기2
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);								//outputimg값 붙이기

	free(outputImg);																		

	fclose(inputFile);
	fclose(outputFile);

	free(Y1);

	printf("\ncode end\n");																	//코드 종료 확인
	return 0;
}