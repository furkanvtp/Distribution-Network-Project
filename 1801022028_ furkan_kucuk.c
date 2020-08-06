#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define POWCONST 2
#define SIZE1 3000
#define SIZE2 100
#define COLUMN1 4
#define COLUMN2 5
#define MESKEN 1
#define TICARI 2
#define SANAYI 3
#define EXIT 6

typedef struct download_center
{
	int id;
	double x1;
	double y1;
	double x2;
	double y2;
}download_center;

typedef struct customer
{
	int id;
	int type;
	double x1;
	double y1;
	double x2;
	double y2;
}customer;

typedef struct line
{
	int id;
	double x1;
	double y1;
	double x2;
	double y2;
}line;

void readFromFile(download_center *center,line lines[SIZE1],customer customers[SIZE1],int *customerCount,int *lineCount);
void printToScreen(download_center center, customer customers[SIZE2], line lines[SIZE2],int customerCount,int lineCount);
void writeToFileBrokenLineList(download_center center,line lines[SIZE1],int brokenLineIds[SIZE1],int lineCount,int *brokenLineCount);
void writeToFileBirdFlightDistances(download_center center,customer customers[SIZE1],int customerCount);
void writeToFileCustomerLineDistance(download_center center,line lines[SIZE1],customer customers[SIZE1],int customerCount);
int customerTypeCounter(customer customers[SIZE2], int customerType, int capacity);
double calculate_BirdFlightDistance(download_center center,customer currentCustomer);
double calculateTotalLineDistance(line lines[SIZE2], int capacity);
void printCenterDetails(download_center center,int id);
void printCustomerDetails(customer customers[SIZE2],int customerCount,int id);
void printLineDetails(line lines[SIZE2],int lineCount, int id);

int main(int argc, char const *argv[])
{
	download_center center;
	line lines[SIZE1];
	customer customers[SIZE1];
	int brokenLineIds[SIZE1],customerCount,lineCount,brokenLineCount=0;
	
	readFromFile(&center,lines,customers,&customerCount,&lineCount);
	writeToFileBrokenLineList(center,lines,brokenLineIds,lineCount,&brokenLineCount);
	writeToFileBirdFlightDistances(center,customers,customerCount);
	writeToFileCustomerLineDistance(center,lines,customers,customerCount);
	printToScreen(center,customers,lines,customerCount,lineCount);	
	return 0;
}

void readFromFile(download_center *center,line lines[SIZE1],customer customers[SIZE1],int *customerCount,int *lineCount){
	FILE *fp;
	int i=0,status=1,flag=1,first=1,endCount=0;
	char new_line_char,arr[SIZE2],buffer1[SIZE2],buffer2[SIZE2],buffer3[SIZE2],buffer4[SIZE2];
	strcpy(buffer1,"INDIRICI_ID");
	strcpy(buffer2,"MUSTERI_ID");
	strcpy(buffer3,"HAT_ID");
	strcpy(buffer4,"END");
	fp = fopen("C:\\Users\\furka\\Desktop\\1.2\\Algoritma\\Proje 1\\IEEE_test_system_12_kopuk.txt", "r");
	while(endCount!=3){
		fscanf(fp,"%s",arr);
		if(strcmp(arr,buffer1)==0){
			status=1;
		}
		else if(strcmp(arr,buffer2)==0){
			status=2;
		}
		else if(strcmp(arr,buffer3)==0){
			status=3;
		}
		if(status==1){
			fscanf(fp,"%[^\n]s",arr);
			fscanf(fp,"%d %lf %lf %lf %lf",&center->id,&center->x1,&center->y1,&center->x2,&center->y2);
			fscanf(fp,"%s",arr);
			endCount++;
		}
		else if(status==2){
			while(flag==1){
				if(first==1){
					fscanf(fp,"%[^\n]s",arr);
					first=0;
				}
				fscanf(fp,"%d %d %lf %lf %lf %lf",&customers[i].id,&customers[i].type,&customers[i].x1,&customers[i].y1,&customers[i].x2,&customers[i].y2);
				i++;
				fscanf(fp,"%s",arr);
				if(strcmp(arr,buffer4)==0){
					flag=0;
				}
				else{
					new_line_char='0';
					while(new_line_char!='\n'){
						fseek(fp,-2,SEEK_CUR);
						fscanf(fp,"%c",&new_line_char);
					}
				}
			}
			(*customerCount)=i; i=0; first=1; flag=1; endCount++;
		}
		else if(status==3){
			while(flag==1){
				if(first==1){
					fscanf(fp,"%[^\n]s",arr);
					first=0;
				}
				fscanf(fp,"%d %lf %lf %lf %lf",&lines[i].id,&lines[i].x1,&lines[i].y1,&lines[i].x2,&lines[i].y2);
				i++;
				fscanf(fp,"%s",arr);
				if(strcmp(arr,buffer4)==0){
					flag=0;
				}
				else{
					new_line_char='0';
					while(new_line_char!='\n'){
						fseek(fp,-2,SEEK_CUR);
						fscanf(fp,"%c",&new_line_char);
					}
				}
			}
			(*lineCount)=i; i=0; first=1; flag=1; endCount++;
		}
	}
	fclose(fp);
}

void printToScreen(download_center center, customer customers[SIZE2], line lines[SIZE2],int customerCount,int lineCount){
	int selection,temp_id;
	do{
		printf("1)Toplan Hat Sayisi ve Toplam Hat Uzunlugu\n");
		printf("2)Toplan Musteri Sayisi ve Musteri Tiplerine Gore Musteri Sayisi\n");
		printf("3)Hat Bilgisi Sorgulama\n");
		printf("4)Musteri Bilgisi Sorgulama\n");
		printf("5)Indirici Merkez Bilgisi Sorgulama\n");
		printf("6)Programi Kapat\n");
		scanf("%d",&selection);
		
		switch(selection){
			case 1:		{
						printf("%d hat %.2lf m\n",lineCount,calculateTotalLineDistance(lines,lineCount));
						break;
						}
			case 2: 	{
						printf("%d musteri %d mesken, %d ticari, %d sanayi\n",customerCount,customerTypeCounter(customers,MESKEN,customerCount),customerTypeCounter(customers,TICARI,customerCount),customerTypeCounter(customers,SANAYI,customerCount));
						break;
						}
			case 3: 	{
						printf("Hat ID?:");
						scanf("%d",&temp_id);
						printLineDetails(lines,lineCount,temp_id);
						break;
						}
			case 4: 	{
						printf("Musteri ID?:");
						scanf("%d",&temp_id);
						printCustomerDetails(customers,customerCount,temp_id);
						break;
						}
			case 5: 	{
						printf("Indirici Merkez ID?:");
						scanf("%d",&temp_id);
						printCenterDetails(center,temp_id);
						break;
						}
			case EXIT: 	{
						break;
						}
		}
	}while(selection!=EXIT);
	return;
}

void writeToFileBrokenLineList(download_center center,line lines[SIZE1],int brokenLineIds[SIZE1],int lineCount,int *brokenLineCount){
	int i=0,j=0,t=0,status=0;
	FILE *fp;
	fp=fopen("Kopuk_hat_listesi.txt","w+");
	while(i<lineCount){
		if(lines[i].x1==center.x1 && lines[i].y1==center.y1) status=1;
		while(j<lineCount){
			if(j!=i && lines[i].x1==lines[j].x2 && lines[i].y1==lines[j].y2){
				status=1;
			}
			j++;
		}
		if(status==0){
			brokenLineIds[t]=lines[i].id;
			t++;
		}
		status=0;
		j=0;
		i++;
	}
	t--;
	while(t>=0){
		fprintf(fp,"Hat %d\n",brokenLineIds[t]);
		t--;
	}
	fclose(fp);
	return;
}

void writeToFileBirdFlightDistances(download_center center,customer customers[SIZE1],int customerCount){
	int i=0;
	FILE *fp;
	fp = fopen("Musteri_kus_ucusu_mesafe.txt", "w+");
	fprintf(fp,"ID\tTIP\tKUS_UCUSU_MESAFE\n");
	while(i<customerCount){
		fprintf(fp,"%d\t%d\t%.2lf\n",customers[i].id,customers[i].type,calculate_BirdFlightDistance(center,customers[i]));
		i++;
	}
	fclose(fp);
	return;
}

void writeToFileCustomerLineDistance(download_center center,line lines[SIZE1],customer customers[SIZE1],int customerCount){
	int i=0,j=0,t=0,k=0,lineNumber=0,lineNumbers[SIZE1],status=0,temp3,temp4;
	double totalDistance=0,x,y,temp1,temp2,temp_distance;
	char array[SIZE2],tempstring[SIZE2];
	FILE *fp1,*fp2;
	fp1 = fopen("Musteri_hat_uzunlugu.txt", "w+");
	fprintf(fp1,"ID\tTIP\tHAT_UZUNLUGU\tKUS_UCUSU_MESAFE\n");
	while(i<customerCount){
		if(customers[i].type==MESKEN) strcpy(array,"Mesken");
		else if(customers[i].type==TICARI) strcpy(array,"Ticari");
		else if(customers[i].type==SANAYI) strcpy(array,"Sanayi");
		strcpy(tempstring,array);
		sprintf(array, "%s_%d_hat_guzergahi.txt",array,customers[i].id);
		fp2=fopen(array,"w+");
		x=customers[i].x1;
		y=customers[i].y1;
		while(x!=center.x1 && y!=center.y1){
			while(lines[j].x2!=x && lines[j].y2!=y){
				j++;
			}
			temp1=lines[j].x1-lines[j].x2;
			temp2=lines[j].y1-lines[j].y2;
			temp_distance=pow(temp1,POWCONST)+pow(temp2,POWCONST);
			temp_distance=sqrt(temp_distance);
			totalDistance+=temp_distance;
			x=lines[j].x1;
			y=lines[j].y1;
			lineNumber=lines[j].id;
			lineNumbers[t]=lineNumber;
			t++;
			j=0;
		}
		t--;
		fprintf(fp2,"Indirici Merkez %d\n",center.id);
		while(t>=0){
			fprintf(fp2,"Hat %d\n",lineNumbers[t]);
			t--;
		}
		fprintf(fp2,"%s %d",tempstring,customers[i].id);
		fclose(fp2);
		fprintf(fp1,"%d\t%d\t%.2lf\t\t%.2lf\n",customers[i].id,customers[i].type,totalDistance,calculate_BirdFlightDistance(center,customers[i]));
		status=0;
		totalDistance=0;
		k=0;
		t=0;
		i++;
	}
	fclose(fp1);
	return;
}

int customerTypeCounter(customer customers[SIZE2], int customerType, int capacity){
	int i=0,totalCount=0;
	while(i<capacity){
		if(customers[i].type==customerType){
			totalCount++;
		}
		i++;
	}
	return totalCount;
}

double calculate_BirdFlightDistance(download_center center,customer currentCustomer){
	double distance,temp1,temp2;
	temp1=currentCustomer.x1-center.x1;
	temp2=currentCustomer.y1-center.y1;
	distance=pow(temp1,POWCONST)+pow(temp2,POWCONST);
	distance=sqrt(distance);
	return distance;
}

double calculateTotalLineDistance(line lines[SIZE2], int capacity){
	int i=0;
	double distance,totalLineDistance=0,temp1,temp2;
	while(i<capacity){
		temp1=lines[i].x1-lines[i].x2;
		temp2=lines[i].y1-lines[i].y2;
		distance=(temp1*temp1)+(temp2*temp2);
		distance=sqrt(distance);
		totalLineDistance+=distance;
		i++;
	}
	return totalLineDistance;
}

void printCenterDetails(download_center center,int id){
	if(center.id!=id)	printf("Bu indirici merkez numarasinda bir merkez bulunmamaktadir!\n");
	else printf("%d, (%.2lf, %.2lf, %.2lf, %.2lf)\n",center.id,center.x1,center.y1,center.x2,center.y2);
	return;
}

void printCustomerDetails(customer customers[SIZE2],int customerCount,int id){
	int i=0,flag=0;
	char customerType[SIZE2];
	while(i<customerCount){
		if(customers[i].id==id){
			flag=1;
			break;
		}
		i++;
	}

	if(customers[i].type==MESKEN) strcpy(customerType,"MESKEN");
	else if(customers[i].type==TICARI) strcpy(customerType,"TICARI");
	else if(customers[i].type==SANAYI) strcpy(customerType,"SANAYI");

	if(flag==0)	printf("Bu musteri numarasinda bir hat bulunmamaktadir!\n");
	else printf("%d, %s, (%.2lf, %.2lf, %.2lf, %.2lf)\n",customers[i].id,customerType,customers[i].x1,customers[i].y1,customers[i].x2,customers[i].y2);
	return;
}

void printLineDetails(line lines[SIZE2],int lineCount, int id){
	int i=0,flag=0;
	while(i<lineCount){
		if(lines[i].id==id){
			flag=1;
			break;
		}
		i++;
	}
	if(flag==0)	printf("Bu hat numarasinda bir hat bulunmamaktadir!\n");
	else printf("Hat %d, (%.2lf, %.2lf, %.2lf, %.2lf)\n",lines[i].id,lines[i].x1,lines[i].y1,lines[i].x2,lines[i].y2);
	return;
}
