#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<math.h>
//학번:17011784 이름:최휘민
//mean 필터 함수부분
void mean(unsigned char* img, int x, int y, int stride, int mode) {		//인자는 이미지, x, y 좌표, stride값과 모드값
	unsigned char	Array[9], tmp = 0, mid = 0;							//x,y와 주변의 3x3의 확인하기 위한 배열Array[9]선언 
	double sum = 0;														//Y(0~255)의 값들을 더하기위한 double형식의 sum 선언
	for (int j = x - 1; j <= x + 1; j++) {								//for문으로 인자 j 를 (x-1, x, x+1) 값을 가지게 선언
		for (int i = y - 1; i <= y + 1; i++) {							//for문으로 인자 i 를 (y-1, y, y+1) 값을 가지게 선언
			if ((j >= 0 && j <= 511) && (i >= 0 && i <= 511)) {			//좌표를 의미하는 i와 j의 값이 [0, 511] 값을 가질때만 작동하는 조건문
				Array[(int)tmp++] = img[j * stride + 3 * i];			//Array배열에 이미지값(Y)를 저장하고 tmp++로 실행한 횟수를 알 수 있다.
			}
		}
	}																	//필요없을수도 있지만 형식이 다른 계산을 할때 대부분 (int), (unsigned char)를 붙여 사용함

	if ((int)tmp < 9) {													//조건문: 만약 tmp가 9보다 작다면? 이는 해당 좌표가  범위를 넘어간 것
		sum = 0;														//넘어간 영역을 처리하기 위한 과정으로 일단 sum=0할당
		for (int i = 0; i < (int)tmp; i++) {							//for문 i < tmp로 Array배열이 가지고있는 값의 수 만큼 작동
			sum += Array[i];											//Array[i]의 값들을 sum에 각각 더해준다
		}
		mid = (unsigned char)(sum / (int)tmp);							// sum/tmp로 평균을 구하고 그값을 mid 값에 할당
		for (int i = (int)tmp; i < 9; i++) {							//i=tmp 부터 i<9 까지 조건으로 Array[9] 배열의 빈 공간만큼 실행
			Array[(int)tmp++] = mid;									//Array의 빈 데이터 공간을 전부 mid 값으로로 할당
		}
	}

	sum = 0;															//sum 재사용을위한 sum=0 다시선언

																		//모드 나눈 이유는 레포트에 설명
	if (mode == 1) {													//모드 확인부분 모드값이 1일 때는 오리지널 mean필터를 적용
		for (int i = 0; i < 9; i++) {									//for문으로 [0:9]범위 실행
			sum += Array[i];											//sum에 각각의 Array[i] 값들을 저장
		}
		mid = (unsigned char)(sum / 9);									//mid에 갑들의 평균값인 sum/9 값을 저장
	}

	else if (mode == 2) {												//모드 값이 2일때는 변형한 mean 필터 사용
		for (int i = 0; i < 9; i++) {									//Array[9]를 정렬하기 위해 for문 선언
			for (int j = 0; j < 9 - i - 1; j++) {						//정렬방법을 버블정렬로 구현하기위해 for문을 하나더 선언
				if (Array[j] > Array[j + 1]) {							//오름차순으로 정렬할려는 조건문
					tmp = Array[j];										//위에서 사용한 배열의 값 개수인 tmp 변수를 재사용 해 Array[j] 저장
					Array[j] = Array[j + 1];							//Array의[j] 에 [j+1]값을 저장
					Array[j + 1] = tmp;									//[j+1]에는 저장해둔 tmp의 값을 저장
				}
			}
		}																//for문이 다 끝나면 Array배열은 오름차순으로 정렬이 완료

		for (int i = 1; i < 8; i++) {									//for문으로 [1:8]범위 실행, 정렬을 했기때문에 가장 최솟값과 최대값은 실행 안함
			sum += Array[i];											//최소값과 최대값을 뺀 나머지 값들을 더해서 sum에 저장
		}
		mid = (unsigned char)(sum / 7);									//mid에 값들의 평균값인 sum/7 값을 저장
	}

	img[x * stride + 3 * y + 0] = mid;									//이제 이미지의 값에 mid값을 저장하면서 함수종료
	img[x * stride + 3 * y + 1] = mid;									//RGB 형식이지만 YCbCr로 표현하기위해 추가로 mid저장
	img[x * stride + 3 * y + 2] = mid;									//똑같이 mid값 저장
}

//main함수 부분
int main() {
	BITMAPFILEHEADER bmpFile;											//파일을 읽어오기위한 과정
	BITMAPINFOHEADER bmpInfo;											//과정은 강의내용과 동일

	//파일1
	FILE* inputFile = NULL;												//FILE형식인 inputFile 선언
	inputFile = fopen("Ou3.bmp", "rb");
	//inputFile = fopen("AICenterY_CombinedNoise.bmp", "rb");				//inputFile에 bmp파일 fopen
	if (inputFile == NULL) {											//파일없을때 실행
		printf("no file1");												//프린트 no file1
		return 0;
	}
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);			//해더읽기1
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);			//해더읽기2

	int width = bmpInfo.biWidth;										//width값저장
	int height = bmpInfo.biHeight;										//height값저장
	int size = bmpInfo.biSizeImage;										//size값저장
	int bitCnt = bmpInfo.biBitCount;									//bitCnt값저장
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;					//Stride값저장
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n", width, stride, height, size, bitCnt);	//저장된 정보 확인용

	unsigned char* inputImg = NULL;										//이미지값 읽기위해 inputImg선언
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));		//size만큼 동적할당
	fread(inputImg, sizeof(unsigned char), size, inputFile);			//size만큼 inputImg에 값 저장

	unsigned char* outputImg = NULL;									//이미지 저장을 위해 outputImg선언
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));	//size만큼 동적할당

	
	FILE* inputFile2 = NULL;											//MSE와 PSNR을 알기위한부분
	inputFile2 = fopen("O3.bmp", "rb");						//파일 입력이 하나로 제한되어 전부 주석처리
	if (inputFile2 == NULL) {											//주석 닫기와 열기만 전부 제거하면 바로실행 가능
		printf("no file2");
		return 0;
	}
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile2);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile2);
	unsigned char* inputImg2 = NULL;
	inputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg2, sizeof(unsigned char), size, inputFile2);

	for (int j = 0; j < height; j++) {									//for j [0~511]영역반복
		for (int i = 0; i < width; i++) {								//for i [0~511]영역반복 총 512*512 번 실행

			if (((i >= 1) && (i <= 510)) && ((j >= 1) && (j <= 510))) {	//이미지의 각 끝영역 빼고 실행
				mean(inputImg, j, i, stride, 2);						//좌표값 j, i 이 끝영역이 아니면 모드 2 실행
			}

			else {
				mean(inputImg, j, i, stride, 1);						//좌표값 j, i 이 끝 영역이면 모드 1 실행
			}

			outputImg[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 0];	//함수median으로 바뀐
			outputImg[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1];	//inputImg값을 각각
			outputImg[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 2];	//outputImg값에 저장
		}
	}

	
	double mse = 0, psnr, * Y1, * Y2;									//MSE와 PSNR을 알기위한부분2
	Y1 = (double*)calloc(width * height, sizeof(double));				//파일 입력이 하나로 제한되어 전부 주석처리
	Y2 = (double*)calloc(width * height, sizeof(double));				//주석 닫기와 열기만 전부 제거하면 바로실행 가능
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
	

	FILE* outputFile = fopen("17011784_최휘민.bmp", "wb");				//outFile을 "wb"모드로 fopen
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);			//해더붙이기1
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);			//해더붙이기2
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);			//outputImg데이터를 outFile에 붙이기
	free(outputImg);													//동적할당 풀기
	fclose(outputFile);													//파일닫기

	free(inputImg);														//동적할당 풀기
	fclose(inputFile);													//파일닫기

	return 0;
}