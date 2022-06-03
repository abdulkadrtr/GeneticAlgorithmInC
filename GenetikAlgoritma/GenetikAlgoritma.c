#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define MAX 6000

void mesafeHesapla(double **populasyon,int aramaUzayi,int n); //Rotanýn toplam uzunlugunu hesaplar ve matriste rotanýn en sonuna yerlestirir.
void swap(double *x,double *y);
void randomAtaKromozomOlustur(double **populasyon,int size,int n);
void enIyiSecimi(double **populasyon,int secim,int aramaUzayi,int size,int n);
void caprazlaFunc(double **populasyon,int secim,int n);
void mutasyonFunc(double **populasyon,int secim,int n);
void matrisYazdir(double **populasyon,int size, int n);
void aramaFunc(double **populasyon,int aramaUzayi,int n);
void nesilAnalizFunc(double **populasyon);
void ataBireyAnalizFunc(double **populasyon);

int main(){
	srand(time(NULL));
	double **populasyon;	
	int i,n,size,secim,iterasyon=0,nesil,mod;
	//509x509 matristen buyuk matris olusturmak icin asagýdaki kod blokunu kullanýyorum. Makrolu tanýmlamada hata veriyor.
    populasyon=(double**) calloc(6000,sizeof(double*));
    for(i=0;i<6000;i++){
        populasyon[i]=(double*) calloc(6000,sizeof(double));
    }
    printf("1 Yazilimi Calistir\n\n2 Analiz Modu (Nesil Analizi) Calistir\n\n3 Analiz Modu (Ata Birey Sayisi Analizi) Calistir\n\nSeciminiz: ");
    scanf("%d",&mod);
    if(mod==1){
		printf("Kac nokta icin rota hesaplanacak: ");
		scanf("%d",&n);
		printf("Baslangic populasyon buyuklugu: ");
		scanf("%d",&size);
		printf("Baslangýc populasyonundan kac ata birey kullanilacak: ");
		scanf("%d",&secim);
		printf("Problem icin kac nesil ilerlenecek: ");
		scanf("%d",&nesil);
		for(i=0;i<n;i++){                  //Butun noktalarin koordinat bilgilerini aliyorum.
			populasyon[0][i]=i+1;
			printf("%d. nokta icin (x,y):",i+1);
			scanf("%lf,%lf",&populasyon[1][i],&populasyon[2][i]);
		}
		randomAtaKromozomOlustur(populasyon,size,n);
		//printf("Random Ata Kromozomlar Olusturuldu: \n\n");
		//matrisYazdir(populasyon,size,n);
		//Buradan itibaren populasyon matrisinde populasyon buyuklugu(size) kadar rota ve bunlarýn uzunluk degeri bulunuyor.
		enIyiSecimi(populasyon,secim,size*3,size,n);
		//printf("En iyileri secildigini gosteren matris:\n");
		//matrisYazdir(populasyon,size,n);
		//Nesil sayisi kadar dongüye girip islemleri baslatýyorum.
		printf("Baslangic populasyonu en iyi sonucu\n");
		mesafeHesapla(populasyon,secim*3,n);							
		aramaFunc(populasyon,secim*3,n);  
		while(iterasyon<nesil){
			if(iterasyon>0){                    //ilk iterasyondan sonra cocuk ve ata bireylerin bulundugu matris icin mesafe hesapliyor.
				mesafeHesapla(populasyon,(secim*9)/2,n);
				//printf("Mutasyon asamasý tamamlandi: \n\n");
				//matrisYazdir(populasyon,size,n);
				//printf("En iyileri secimi gerceklesti\n");
				// SEÇÝM AÞAMASI BASLANGICI (En kýsa mesafeye sahip secim adetinde rota(kromozom) secilecek.							
				enIyiSecimi(populasyon,secim,(9*secim)/2,size,n);
				//SECÝM ASAMASI BÝTÝSÝ				
				//matrisYazdir(populasyon,size,n);	
			}
			//CAPRAZLAMA ASAMASI BASLANGICI
			caprazlaFunc(populasyon,secim,n);	
			//printf("Caprazlama asamasi tamamlandi: \n\n");
			//matrisYazdir(populasyon,size,n);	
			//CAPRAZLAMA ASAMASI BÝTÝSÝ
			//MUTASYON ASAMASI BASLANGIC                      
			mutasyonFunc(populasyon,secim,n); //cocuk bireyleri mutasyona ugratiyor					
			//printf("Mutasyon asamasý tamamlandý: \n\n");
			//matrisYazdir(populasyon,size,n);
			//MUTASYON ASAMASI BÝTÝSÝ	
			iterasyon++;
		}
		mesafeHesapla(populasyon,(secim*9)/2,n);
		//printf("Sonuc Matris \n\n");
		//matrisYazdir(populasyon,secim+(secim/2),n);
		//printf("\n\n");
		printf("Islemler sonucu olusan populasyon en iyi sonucu:\n");
		aramaFunc(populasyon,(9*secim)/2,n);   	
	}
	else if(mod==2){
		nesilAnalizFunc(populasyon);
	}
	else if(mod==3){
		ataBireyAnalizFunc(populasyon);
	}
	else{
		printf("Hatali deger girdiniz.");
	}	
	return 0;
}


void matrisYazdir(double **populasyon,int size, int n){
	int i,j;
	for(i=0;i<size*3;i++){
		for(j=0;j<=n+1;j++){
			printf("%.0lf ",populasyon[i][j]);
		}
		printf("\n");
	}	
}

void mesafeHesapla(double **populasyon,int aramaUzayi,int n){
	int i,k;  //Bir kromozom icin tüm noktalar arasýndaki uzaklýgý hesaplayýp kromozomun en sonuna yerleþtiriyor.
	double tempx,tempy,temp,mesafe;
	for(k=0;k<aramaUzayi;k+=3){
		tempx=0,tempy=0,temp=0,mesafe=0;
		for(i=1;i<n;i++){                                  
			tempx=populasyon[k+1][i]-populasyon[k+1][i-1];  //2 nokta arasi uzaklýk hesaplama bloku
			tempx=tempx*tempx;
			tempy=populasyon[k+2][i]-populasyon[k+2][i-1];
			tempy=tempy*tempy;
			temp=tempx+tempy;
			temp=sqrt(temp);
			mesafe+=temp;	
		}
		populasyon[k][n]=mesafe;	
	}
}

void enIyiSecimi(double **populasyon,int secim,int aramaUzayi,int size,int n){
	int j,i,k,min,indis;
	for(j=0;j<secim;j++){  //Secim degiskeni adetinde en kýsa mesafeye sahip rotayý secip isaretliyorum
		min=99999999;   
		for(i=0;i<aramaUzayi;i+=3){
			if(min>populasyon[i][n]&&populasyon[i][n]!=0){
				min=populasyon[i][n];
				indis=i;
			}
		}
		populasyon[indis][n]=0;   //ayný min degerini bulmasýn diye ilgili mesafeyi sýfýrlýyorum.
		populasyon[indis][n+1]=1;    // oldurulmeyecek olan kromozomlarý 1 ile isaretliyorum.
	}
	//printf("Secilen Kromozomlar 1 ile isaretlendi: \n\n");
	//matrisYazdir(populasyon,size,n); 			
	k=0;
	for(i=0;i<aramaUzayi;i+=3){         //Bu blokta 1 ile isaretledigim rotalarý bulup bunlarý sirasiyla en üstten baslayarak matrisin altina dogru diziyorum.
		if(populasyon[i][n+1]==1){ //islem sonucunda secmek istedigim sayida kromozom matrisin en ustune dizilmis oluyor.
			for(j=0;j<n+2;j++){
				//printf("tasinacak deger :%lf\n",populasyon[i+1][j]);
				//printf("tasinan yer   %lf\n",populasyon[k+1][j]);
				populasyon[k][j]=populasyon[i][j];
				populasyon[k+1][j]=populasyon[i+1][j];				
				populasyon[k+2][j]=populasyon[i+2][j];				
			}
			k+=3;
		}
	}	
	for(i=secim*3;i<MAX;i++){     //Burada populasyondaki secilmis kromozlar haricindeki kromozomlarý yok ediyorum.
		populasyon[i-(secim*3)][n+1]=0; //isaretleme yaptýgým birleri de burada 0a ceviyorum.
		for(j=0;j<n+2;j++){ 
			populasyon[i][j]=0;	
		}
	}
	//printf("1 ile isaretlenen kromozomlar secilerek diger kromozomlar yok edildi:\n\n");
	//matrisYazdir(populasyon,size,n);				
}

void caprazlaFunc(double **populasyon,int secim,int n){
	int i,j,k,m,l,flag=0,temp;
	k=0;									//CrossOver asamasý (tek noktalý çaprazlama) Kromozomun yarýsý bir ata kromozomdan, diger yarýsý digerinden.
	for(i=0;i<(secim*3)-3;i+=6){          //Bu blok cocuk kromozomun ilk yarýsýný alýyor.Yeni bireyler matriste aþaðýya diziliyor.
		for(j=0;j<n/2;j++){
			populasyon[secim*3+k][j]=populasyon[i][j];
			populasyon[secim*3+k+1][j]=populasyon[i+1][j];
			populasyon[secim*3+k+2][j]=populasyon[i+2][j];
		}
		k+=3;	
	}
	k=0;									
	for(i=3;i<secim*3;i+=6){          //Bu blok cocuk kromozomun ikinci yarýsýný alýyor.
		temp=n/2; // temp degiskeni eger aday elemaným kromozomun ilk kýsmýnda bulunuyorsa onu alamayacaðýmdan sýranýn kaymasýný engeller.Boylece alamadýgým elemanlar icin kromozomun sonunda yerim olur.
		for(j=n/2;j<n;j++){
			populasyon[secim*3+k][j]=populasyon[i][j];
			for(l=0;l<n/2;l++){
				if(populasyon[i+1][j]==populasyon[secim*3+k+1][l]&&populasyon[i+2][j]==populasyon[secim*3+k+2][l]){ //2.kromozomdan aldýgýn noktayý ilk kromzomdan almadýysam cocuk kromozoma aktarýyorum					
					flag=1;					
				}				
			}
			if(flag!=1){
				populasyon[secim*3+k+1][temp]=populasyon[i+1][j];
				populasyon[secim*3+k+2][temp]=populasyon[i+2][j];
				temp++;	
			}
			flag=0;		
		}
		k+=3;	
	}
	flag=0;
	//2. Kromozomun sonran yarýsýný aldým ve bu parcada olup cocuk kromozomun ilk yarýsýnda olmayan noktalarý cocuk kromozoma ekledim.
	//alt blokta 2. kromozomun basýndan baslayýp cocuk kromozomda olmayan noktalarý cocuk kromozoma ekliyorum. Boylece caprazlama islemi bitiyor. 
	for(i=secim*3;i<(secim*9)/2;i+=3){
		temp=n/2;
		for(j=n/2;j<n;j++){                 // kromozomda 0,0 olan yer ilk nerde baslýyor onu buluyorum.		
			if(populasyon[i+1][j]!=0||populasyon[i+2][j]!=0){
				temp++;
			}
		}
		for(k=3;k<secim*3;k+=6){
			for(m=0;m<n;m++){	
				for(l=0;l<temp;l++){
					if(populasyon[k+1][m]==populasyon[i+1][l]&&populasyon[k+2][m]==populasyon[i+2][l]){
						flag=1;
					}			
				}
				if(flag!=1){									
					populasyon[i+1][temp]=populasyon[k+1][m];
					populasyon[i+2][temp]=populasyon[k+2][m];
					temp++;
				}
				flag=0;					
			}
		}	
	}
	//printf("\n\n");
	//matrisYazdir(populasyon,secim+(secim/2),n);
}

void mutasyonFunc(double **populasyon,int secim,int n){
	int i,j,random1,random2;
	for(i=secim*3;i<(9*secim)/2;i+=3){
		for(j=0;j<(n*1)/10;j++){  //Her kromozom için mutasyon oraný kadar çalýþacak olan döngü Oran %10
			random1= rand() % n;
			random2= rand() % n;
			swap(&populasyon[i+1][random1],&populasyon[i+1][random2]);
			swap(&populasyon[i+2][random1],&populasyon[i+2][random2]);	
		}
	}			
}

void randomAtaKromozomOlustur(double **populasyon,int size,int n){
	int i,j,random1,random2;
	for(i=3;i<size*3;i+=3){   //Burada populasyon büyüklügü kadar ata kromozomu(girilen noktalarý) kopyalýyorum. Elimde populasyon kadar es kromozom olusacak
		for(j=0;j<n;j++){							
			populasyon[i][j]=populasyon[0][j];
			populasyon[i+1][j]=populasyon[1][j];
			populasyon[i+2][j]=populasyon[2][j];
		}	
	}
	for(i=0;i<size*3;i+=3){    //Random ata kromozom populasyonu olusturmak icin kopyaladýgým kromozomlardan rastgele 2 nokta seciyorum ve o noktalarý
		for(j=0;j<n;j++){     //swap ediyorum(Her kromozomu n defa). Boylece elimde rastgele dizilime sahip kromozomlar olusuyor.
			random1=rand() % n+1;
			random2=rand() % n+1;			
			swap(&populasyon[i+1][random1-1],&populasyon[i+1][random2-1]);
			swap(&populasyon[i+2][random1-1],&populasyon[i+2][random2-1]);
		}			
	}
	mesafeHesapla(populasyon,size*3,n);	
}

void swap(double *x,double *y){
	double temp;
	temp=*x;
	*x=*y;
	*y=temp;
}

void aramaFunc(double **populasyon,int aramaUzayi,int n){
	int i,indis,min=999999999;
	for(i=0;i<aramaUzayi;i+=3){
		if(populasyon[i][n]<min){
			min=populasyon[i][n];
			indis=i;
		}					
	}
	printf("\nGidilecek Rota Uzunlugu: %lf\n",populasyon[indis][n]);
	for(i=0;i<n;i++){
		printf("(%0.lf,%0.lf) -> ",populasyon[indis+1][i],populasyon[indis+2][i]);
		if(i==8||i==16||i==24){
			printf("\n");			
		}
	}
	printf("| Rota Bitis \n");
}

void nesilAnalizFunc(double **populasyon){
	int size=800,n=30,secim=40,j,i,temp=0,nesil=0,mod=0,iterasyon;
	printf("Nesil sayisina gore zaman karmasikligi analizi\n");
	printf("Degerler:\n\nBaslangic Populasyon Buyuklugu: 800\n\nKullanilacak Ata Birey Sayisi:40\n\nKullanilan nokta sayisi:30\n\nNesil Sayisi:500 - 1000 - 1500\n\n");
	double sure[3]={0};
{	populasyon[1][0]=1;
	populasyon[2][0]=25;
	populasyon[1][1]=42;
	populasyon[2][1]=115;
	populasyon[1][2]=19;
	populasyon[2][2]=29;
	populasyon[1][3]=35;
	populasyon[2][3]=41;
	populasyon[1][4]=78;
	populasyon[2][4]=10;
	populasyon[1][5]=20;
	populasyon[2][5]=45;
	populasyon[1][6]=92;
	populasyon[2][6]=99;
	populasyon[1][7]=128;
	populasyon[2][7]=11;
	populasyon[1][8]=21;
	populasyon[2][8]=31;
	populasyon[1][9]=69;
	populasyon[2][9]=52;
	populasyon[1][10]=142;
	populasyon[2][10]=185;
	populasyon[1][11]=12;
	populasyon[2][11]=12;
	populasyon[1][12]=198;
	populasyon[2][12]=202;
	populasyon[1][13]=15;
	populasyon[2][13]=155;
	populasyon[1][14]=45;
	populasyon[2][14]=98;
	populasyon[1][15]=1;
	populasyon[2][15]=10;
	populasyon[1][16]=28;
	populasyon[2][16]=78;
	populasyon[1][17]=42;
	populasyon[2][17]=34;
	populasyon[1][18]=13;
	populasyon[2][18]=111;
	populasyon[1][19]=169;
	populasyon[2][19]=187;
	populasyon[1][20]=45;
	populasyon[2][20]=77;
	populasyon[1][21]=195;
	populasyon[2][21]=63;
	populasyon[1][22]=205;
	populasyon[2][22]=40;
	populasyon[1][23]=267;
	populasyon[2][23]=121;
	populasyon[1][24]=15;
	populasyon[2][24]=155;
	populasyon[1][25]=216;
	populasyon[2][25]=3;
	populasyon[1][26]=42;
	populasyon[2][26]=242;
	populasyon[1][27]=78;
	populasyon[2][27]=19;
	populasyon[1][28]=81;
	populasyon[2][28]=161;
	populasyon[1][29]=179;
	populasyon[2][29]=87;		
	}
	while(mod<3){
		clock_t start_t, end_t;
		start_t = clock();
		nesil+=500;
		printf("------------------------------------------------------\n");
		printf("Nesil sayisi: %d \n\n",nesil);
		randomAtaKromozomOlustur(populasyon,size,n);
		printf("Baslangic Populasyonu En Iyi Sonucu:\n");
		enIyiSecimi(populasyon,secim,3*size,size,n);
		mesafeHesapla(populasyon,secim*3,n);	
		aramaFunc(populasyon,3*secim,n);
		iterasyon=0;
		while(iterasyon<nesil){
			if(iterasyon>0){                    //ilk iterasyondan sonra cocuk ve ata bireylerin bulundugu matris icin mesafe hesapliyor.
				mesafeHesapla(populasyon,(secim*9)/2,n);
				enIyiSecimi(populasyon,secim,(9*secim)/2,size,n);	
			}
			caprazlaFunc(populasyon,secim,n);
			mutasyonFunc(populasyon,secim,n);
			iterasyon++;
		}
		mesafeHesapla(populasyon,(secim*9)/2,n);
		printf("\n\nIslemler Sonucu Olusan Populasyon Havuzu En Iyi Sonucu:\n");
		aramaFunc(populasyon,(9*secim)/2,n);
		end_t = clock();
		sure[mod]=(double)(end_t - start_t) / CLOCKS_PER_SEC;
		printf("\nHesaplanan sure: %f\n", (double)(end_t - start_t) / CLOCKS_PER_SEC);
		printf("------------------------------------------------------");		
		printf("\n");	
		mod++;
	}
	printf("                     ANALIZ                     \n\n");
	printf("------------------------------------------------------\n\n\n");
	for(i=0;i<3;i++){
		temp+=500;
		printf("%d nesil\n",temp);
		for(j=0;j<sure[i]*10;j++){
			printf("*");
		}
		printf("%.3lf(sn)",sure[i]);
		printf("\n\n\n");
	}	
}

void ataBireyAnalizFunc(double **populasyon){
	int size=800,n=30,secim=0,j,i,temp=0,nesil=500,mod=0,iterasyon;
	double sure[4]={0};
	printf("Kullanilacak Ata Birey Sayisina Gore Zaman Karmasikligi Analizi\n");
	printf("Degerler:\n\nBaslangic Populasyon Buyuklugu: 800\n\nKullanilan nokta sayisi:30\n\nNesil Sayisi:500\n\nKullanilacak Ata Birey Sayisi: 40 - 80 - 120 - 160\n\n");

{	populasyon[1][0]=1;
	populasyon[2][0]=25;
	populasyon[1][1]=42;
	populasyon[2][1]=115;
	populasyon[1][2]=19;
	populasyon[2][2]=29;
	populasyon[1][3]=35;
	populasyon[2][3]=41;
	populasyon[1][4]=78;
	populasyon[2][4]=10;
	populasyon[1][5]=20;
	populasyon[2][5]=45;
	populasyon[1][6]=92;
	populasyon[2][6]=99;
	populasyon[1][7]=128;
	populasyon[2][7]=11;
	populasyon[1][8]=21;
	populasyon[2][8]=31;
	populasyon[1][9]=69;
	populasyon[2][9]=52;
	populasyon[1][10]=142;
	populasyon[2][10]=185;
	populasyon[1][11]=12;
	populasyon[2][11]=12;
	populasyon[1][12]=198;
	populasyon[2][12]=202;
	populasyon[1][13]=15;
	populasyon[2][13]=155;
	populasyon[1][14]=45;
	populasyon[2][14]=98;
	populasyon[1][15]=1;
	populasyon[2][15]=10;
	populasyon[1][16]=28;
	populasyon[2][16]=78;
	populasyon[1][17]=42;
	populasyon[2][17]=34;
	populasyon[1][18]=13;
	populasyon[2][18]=111;
	populasyon[1][19]=169;
	populasyon[2][19]=187;
	populasyon[1][20]=45;
	populasyon[2][20]=77;
	populasyon[1][21]=195;
	populasyon[2][21]=63;
	populasyon[1][22]=205;
	populasyon[2][22]=40;
	populasyon[1][23]=267;
	populasyon[2][23]=121;
	populasyon[1][24]=15;
	populasyon[2][24]=155;
	populasyon[1][25]=216;
	populasyon[2][25]=3;
	populasyon[1][26]=42;
	populasyon[2][26]=242;
	populasyon[1][27]=78;
	populasyon[2][27]=19;
	populasyon[1][28]=81;
	populasyon[2][28]=161;
	populasyon[1][29]=179;
	populasyon[2][29]=87;		
	}
	while(mod<4){
		clock_t start_t, end_t;
		start_t = clock();
		secim+=40;
		printf("------------------------------------------------------\n");
		printf("Kullanilacak Ata Birey Sayisi: %d \n\n",secim);
		randomAtaKromozomOlustur(populasyon,size,n);
		printf("Baslangic Populasyonu En Iyi Sonucu:\n");
		enIyiSecimi(populasyon,secim,size*3,size,n);
		mesafeHesapla(populasyon,(secim*3),n);		
		aramaFunc(populasyon,3*secim,n);
		iterasyon=0;
		while(iterasyon<nesil){
			if(iterasyon>0){                    //ilk iterasyondan sonra cocuk ve ata bireylerin bulundugu matris icin mesafe hesapliyor.
				mesafeHesapla(populasyon,(secim*9)/2,n);
				enIyiSecimi(populasyon,secim,(9*secim)/2,size,n);
			}
			caprazlaFunc(populasyon,secim,n);
			mutasyonFunc(populasyon,secim,n);
			iterasyon++;
		}
		mesafeHesapla(populasyon,(secim*9)/2,n);
		printf("\n\nIslemler Sonucu Olusan Populasyon Havuzu En Iyi Sonucu:\n");
		aramaFunc(populasyon,(9*secim)/2,n);
		end_t = clock();
		sure[mod]=(double)(end_t - start_t) / CLOCKS_PER_SEC;
		printf("\nHesaplanan sure: %f\n", (double)(end_t - start_t) / CLOCKS_PER_SEC);
		printf("------------------------------------------------------");		
		printf("\n");	
		mod++;
	}
	printf("                     ANALIZ                     \n\n");
	printf("------------------------------------------------------\n\n\n");
	for(i=0;i<4;i++){
		temp+=40;
		printf("%d Ata Birey\n",temp);
		for(j=0;j<sure[i]*10;j++){
			printf("*");
		}
		printf("%.3lf(sn)",sure[i]);
		printf("\n\n\n");
	}	
}

