#define _CRT_SECURE_NO_DEPRECATE
#include <conio.h>
#include <stdio.h>

void konversia_1(int number[2], char* format);

int main()
{
	int N = 12087;
	int nm_reader = 23;             // kilkist' chytachiv
	FILE* file_rcmnd = fopen("book_recommended.txt", "w");           // rekomendacii
	FILE* file_infmt = fopen("program_information.txt", "w");        // promizhna informacia
	FILE* file_read;
	if ((file_read = fopen("book_edit_1_new.txt", "r")) == NULL)          // dani do obrobky
	{
		printf("ERROR: NO FILE_READ\n");
		return 1;
	}

	int reader_0;
	int temp;
	int nr_reader;
	int book_0[2];
	int ranking;
	int** book;                                // masyv, de zapysani ISBN knyzhok
	int** book_analise;                        // masyv z informacijeju pro knyzhky : 0 - nr chytacha ; 1 - rejtynh ; 2 - kilkist' chytachiv
	book = new int* [N];
	book_analise = new int* [N];
	for (int i = 0; i < N; i++)
	{
		book[i] = new int[2];
		book_analise[i] = new int[3];
	}
	for (int i = 0; i < N; i++)
		book_analise[i][2] = 0;               // kilkist' chytachiv na pochatku = 0
	int* reader = new int[nm_reader];        // masyv, de zapysani USERS chytacha
	for (int i = 0; i < nm_reader; i++)             // zanulujemo, ne zapysuvaty chytacha bahato raziv
		reader[i] = 0;

	int* reader_books_0 = new int[nm_reader];    // dopomichnyj masyv : skilky knyzhok rezenzuvav chytach chytach 
	for (int i = 0; i < nm_reader; i++)
		reader_books_0[i] = 0;
	int nm_book = 0;                              // v majbutniomu kilkist' riznych knyzhok
	int nr_1 = 0;
	for (int i = 1; i <= N; i++)                        // chytajemo dani
	{
		fscanf(file_read, "%d %d %d %d %d %d", &temp, &nr_reader, &reader_0, book_0, book_0 + 1, &ranking);
		reader_books_0[nr_reader]++;              // chytach recenzuvav
		if (reader[nr_reader] == 0)  reader[nr_reader] = reader_0;      // zapysujemo tilky raz, koly ranisze ne zapysaly
		int j;
		for (j = 0; j < nm_book; j++)                                  // prohladajemo chy taka knyzhka ne zapysana ranisze
			if ((book_0[0] == book[j][0]) && (book_0[1] == book[j][1]))  // zapysana ranisze
			{
				book_analise[j][2]++;
				nr_1++;
				break;                                     // zapysana ranisze - vychodymo cherez break	
			}
		if (j == nm_book)                                      // vyjszly ne cherez break : ne zapysana ranisze
		{
			book[nm_book][0] = book_0[0];              // zapamiatovujemo knyzhku i informaciu pro neji
			book[nm_book][1] = book_0[1];
			book_analise[nm_book][0] = nr_reader;
			book_analise[nm_book][1] = ranking;
			book_analise[nm_book][2]++;
			nm_book++;
		}
	}
	fclose(file_read);

	int* reader_books_1 = new int[nm_reader];              // skilky knyzhok recenzuvav tilky cej chytach
	int* reader_books_2 = new int[nm_reader];              // skilky z tych knyzhok majut' rejtynh 10
	int nm_books_1 = 0;
	for (int i = 0; i < nm_reader; i++)
	{
		reader_books_1[i] = 0;
		reader_books_2[i] = 0;
	}
	for (int i = 0; i < nm_book; i++)                           // formujemo vvedeni masyvy z dopomizhnoju informacijeju
	{
		if (book_analise[i][2] == 1)
		{
			int poz_reader = book_analise[i][0];
			reader_books_1[poz_reader]++;
			if (book_analise[i][1] == 10)
				reader_books_2[poz_reader]++;
			nm_books_1++;
		}
	}

	int nm_book_rd = 0;                             // kilkist' knyzhok recenzovanych kilkoma chytachamy
	for (int i = 0; i < nm_book; i++)                     // oznaka : poperednio zapysane chyslo chytachiv - ne 1
		if (book_analise[i][2] != 1) nm_book_rd++;

	fprintf(file_infmt, "                      READERS LIST\n\n");      // vypysujemo perszyj paket promizhoji informacii
	for (int i = 0; i < nm_reader; i++)
	{
		fprintf(file_infmt, "nr: %2d USERS-ID: %6d  NM BOOKS: %4d (%4d, %4d)     ", i, reader[i], reader_books_0[i], reader_books_1[i], reader_books_2[i]);
		if (!((i + 1) % 2)) fprintf(file_infmt, "\n");
	}
	fprintf(file_infmt, "\n\n");
	fprintf(file_infmt, "NUMBER BOOKS                      : %5d\n", nm_book);
	fprintf(file_infmt, "NUMBER BOOKS REVIEDED ONE READER  : %5d\n", nm_books_1);
	fprintf(file_infmt, "NUMBER BOOKS REVIEDED BOTH READERs: %5d\n", nm_book_rd);
	fprintf(file_infmt, "\n__________________________________________________________________________________\n\n\n");

	int** book_rd;                                 // masyv knyzhok recenzovanych kilkoma chytachamy
	book_rd = new int* [nm_book_rd];
	//	int *nm_book_rd_1 = new int [nm_book_rd];      // dla kozhoji takoji knyzhky vidvodymo book_analise[i][2]+3 pozycij pamiati
	int i_0 = 0;
	//	int sum = 0;
	for (int i = 0; i < nm_book; i++)
	{
		if (book_analise[i][2] != 1)               // vydilajemo pamiat dla informacii pro kozhnu taku knyzhku 
		{
			book_rd[i_0] = new int[book_analise[i][2] + 3];
			//			nm_book_rd_1[i_0] = book_analise[i][2]+3;
			i_0++;
			//			sum += book_analise[i][2];
		}
	}
	i_0 = 0;
	for (int i = 0; i < nm_book; i++)
		if (book_analise[i][2] != 1)
		{
			book_rd[i_0][2] = i;                   // nomer knyzhky v poperedniomu masyvi 
			book_rd[i_0][0] = book[i][0];          // dla zruchnosti ISBN knyzhky
			book_rd[i_0][1] = book[i][1];          // majuchy ISBN ne musymo kozhnyj raz zvertatysia do poperednioho masyvu
			i_0++;
		}

	int* pozycia = new int[nm_book_rd];           // v majbutniomu bizhucha pozycia pry zapysi informacii pro knyzhku
	for (int j = 0; j < nm_book_rd; j++)                 // jaku recenzuvaly kilka chytachiv (na pochatku - 3 - tretia komirka)

		pozycia[j] = 3;
	file_read = fopen("book_edit_1_new.txt", "r");    // znovu prohladajemo fajl
	for (int i = 1; i <= N; i++)                         // dla knyzhok recenzovanych kilkoma chytachamy
	{                                              // fiksujemo chytachiv
		fscanf(file_read, "%d %d %d %d %d %d", &temp, &nr_reader, &reader_0, book_0, book_0 + 1, &ranking);
		for (int j = 0; j < nm_book_rd; j++)
			if ((book_0[0] == book_rd[j][0]) && (book_0[1] == book_rd[j][1]))
			{
				book_rd[j][pozycia[j]] = nr_reader;
				pozycia[j]++;
			}
	}
	fclose(file_read);

	int numb[2];                    // vypysujemo druhyj paket promizhnoji informacii
	char format[20];                // dla koznoji knyzky, jaku recenzuvaly kilka chytachiv podajemo jich nomery
	fprintf(file_infmt, "\n           BOOKS REVIEWED BOTH READERS\n\n");
	for (int j = 0; j < nm_book_rd; j++)
	{
		numb[0] = book_rd[j][0];
		numb[1] = book_rd[j][1];
		konversia_1(numb, format);
		fprintf(file_infmt, "j=%3d %11s : ", j + 1, format);
		for (int k = 3; k < pozycia[j]; k++)
			fprintf(file_infmt, "%3d  ", book_rd[j][k]);
		fprintf(file_infmt, "\n");
	}
	fprintf(file_infmt, "\n");
	fprintf(file_infmt, "\n__________________________________________________________________________________\n\n\n");

	int* reader_graf = new int[nm_reader * (nm_reader - 1) / 2];         // formujemo hraf chytachiv
	for (int i = 0; i < nm_reader * (nm_reader - 1) / 2; i++)                   // na rebrach hrafa fiksujemo skilky knyzhok spilno reecenzuvaly
		reader_graf[i] = 0;

	for (int i = 0; i < nm_book_rd; i++)                // prohladajemo masyv knyzhok jaki recenzuvaly kilka chytachiv               
		for (int j = 3; j < pozycia[i]; j++)            // nomery chytachiv z tretioji pozycii masyvu book_rd[i][j]
			for (int k = j + 1; k < pozycia[i]; k++)
			{
				int pozycia_1 = book_rd[i][j];    // fiksujemo vsi mozhlyvi pary riznych chytachiv    
				int pozycia_2 = book_rd[i][k];
				if (pozycia_1 > pozycia_2)          // dla vyznachennia nomera rebra hrafa maje buty pozycia_1<pozycia_2
				{
					int temp_0 = pozycia_1;
					pozycia_1 = pozycia_2;
					pozycia_2 = temp_0;
				}
				int pozycia_graf = (2 * nm_reader - pozycia_1 - 1) * pozycia_1 / 2 + pozycia_2 - pozycia_1 - 1;
				reader_graf[pozycia_graf]++;      // zbilszujemo vidpovidnu komirku hrafa
			}

	fprintf(file_infmt, "\n           NUMBER BOOKS REVIEWED TWO READERS\n\n");   // formujemo tretij paket promizhnoji informacji
	for (int j = 0; j < nm_reader; j++)                             // skilky knyzhok spilno recenzuvaly dva chytach (vsi mozhlyvi pary)
	{
		for (int k = j + 1; k < nm_reader; k++)
		{
			int pozycia_graf = (2 * nm_reader - j - 1) * j / 2 + k - j - 1;
			fprintf(file_infmt, "RD_1:%2d,RD_2:%2d: NM_BK: %2d       ", j, k, reader_graf[pozycia_graf]);
			if (!((pozycia_graf + 1) % 3)) fprintf(file_infmt, "\n");
		}
	}
	fprintf(file_infmt, "\n");
	fprintf(file_infmt, "\n__________________________________________________________________________________\n\n\n");
	fclose(file_infmt);

	for (int j = 0; j < nm_reader; j++)             // formujemo ostatochnu informaciju
	{
		for (int k = j + 1; k < nm_reader; k++)
		{
			int pozycia_graf = (2 * nm_reader - j - 1) * j / 2 + k - j - 1;
			if (reader_graf[pozycia_graf] >= 15)            // obmezhujemos' do par chytachiv jaki recenzuvaly >= 15 knyzhok
			{
				//    		printf("READER_1 USERS-ID: %5d  and  READER_2 USERS-ID : %5d\n\n",reader[j],reader[k]);
				//    		printf("NUMBER BOOKS REVIEWED BOTH READERS = %d\n\n",reader_graf[pozycia_graf]);
				//    		printf("  NR       ISBN      NR       ISBN      NR       ISBN      NR       ISBN\n");
				fprintf(file_rcmnd, "READER_1 USERS-ID: %6d (NR %2d) and  READER_2 USERS-ID %6d (NR %2d) :\n\n", reader[j], j, reader[k], k);
				fprintf(file_rcmnd, "NUMBER BOOKS REVIEWED BOTH READERS = %d\n\n", reader_graf[pozycia_graf]);
				fprintf(file_rcmnd, "  NR       ISBN      NR       ISBN      NR       ISBN      NR       ISBN      NR       ISBN\n");

				int nr_b = 0;
				for (int i_b = 0; i_b < nm_book_rd; i_b++)        // vypysujemo spysok knyzhok jaki recenzuvala para chychiv
				{
					int flag_1 = 0;
					int flag_2 = 0;
					for (int i_p = 3; i_p < pozycia[i_b]; i_p++)
					{
						if (book_rd[i_b][i_p] == j) flag_1 = 1;
						if (book_rd[i_b][i_p] == k) flag_2 = 1;
					}
					if (flag_1 && flag_2)
					{
						nr_b++;
						numb[0] = book_rd[i_b][0];
						numb[1] = book_rd[i_b][1];
						konversia_1(numb, format);
						//			    	printf("%2d : %11s  ",nr_b,format);
						fprintf(file_rcmnd, "%3d : %11s  ", nr_b, format);
						//			    	if (!(nr_b%4)) printf("\n");
						if (!(nr_b % 5)) fprintf(file_rcmnd, "\n");
					}
				}
				//			printf("\n\n");
				//			printf("BOOKS RECOMMENDED BY THE READER USERS-ID: %5d \n",reader[j]);
				//			printf("  NR       ISBN      NR       ISBN      NR       ISBN      NR       ISBN\n");
				fprintf(file_rcmnd, "\n\n");
				fprintf(file_rcmnd, "BOOKS RECOMMENDED BY THE READER USERS-ID: %6d \n", reader[j]);
				fprintf(file_rcmnd, "  NR       ISBN      NR       ISBN      NR       ISBN      NR       ISBN      NR       ISBN\n");

				nr_b = 0;                          // knyzhky rekomendovani dla pershoho chytacha pary (vid druhoho)
				for (int i_b = 0; i_b < nm_book; i_b++)
				{
					if ((book_analise[i_b][0] == k) && (book_analise[i_b][1] == 10) && (book_analise[i_b][2] == 1))
					{
						nr_b++;
						numb[0] = book[i_b][0];
						numb[1] = book[i_b][1];
						konversia_1(numb, format);
						//	        		printf("%2d : %11s  ",nr_b,format);
						fprintf(file_rcmnd, "%3d : %11s  ", nr_b, format);
						//	        		if (!(nr_b%4)) printf("\n");
						if (!(nr_b % 5)) fprintf(file_rcmnd, "\n");
					}
				}
				//			printf("\n\n");
				//			printf("BOOKS RECOMMENDED BY THE READER USERS-ID: %5d \n",reader[k]);
				//			printf("  NR       ISBN      NR       ISBN      NR       ISBN      NR       ISBN\n");
				fprintf(file_rcmnd, "\n\n");
				fprintf(file_rcmnd, "BOOKS RECOMMENDED BY THE READER USERS-ID: %6d \n", reader[k]);
				fprintf(file_rcmnd, "  NR       ISBN      NR       ISBN      NR       ISBN      NR       ISBN      NR       ISBN\n");

				nr_b = 0;
				for (int i_b = 0; i_b < nm_book; i_b++)        // knyzhky rekomendovani dla druhoho chytacha pary (vid perszoho)
				{
					if ((book_analise[i_b][0] == j) && (book_analise[i_b][1] == 10) && (book_analise[i_b][2] == 1))
					{
						nr_b++;
						numb[0] = book[i_b][0];
						numb[1] = book[i_b][1];
						konversia_1(numb, format);
						//	        		printf("%2d : %11s  ",nr_b,format);
						fprintf(file_rcmnd, "%3d : %11s  ", nr_b, format);
						//	        		if (!(nr_b%4)) printf("\n");
						if (!(nr_b % 5)) fprintf(file_rcmnd, "\n");
					}
				}
				//            printf("\n\n\n\n");
				fprintf(file_rcmnd, "\n\n\n");
				fprintf(file_rcmnd, "_______________________________________________________________________________________________");
				fprintf(file_rcmnd, "\n\n\n");
			}
		}
	}
	fclose(file_rcmnd);

	printf("\n\nEND PROGRAM\n");

	return 0;
}

void konversia_1(int number[2], char* format)     // funkcja jaka peretvorije poslidovnist' dvoch chysel v riadok: ISBN knyzhky
{
	int* number_0 = new int[2];
	number_0[0] = number[0];
	int nm_0 = 1;
	while (number_0[0] / 10 > 0)
	{
		nm_0++;
		number_0[0] = number_0[0] / 10;
	}
	number_0[0] = number[0];
	for (int i = nm_0 - 1; i >= 0; i--)
	{
		format[i] = char(48 + number_0[0] % 10);
		number_0[0] = number_0[0] / 10;
	}
	number_0[1] = number[1];
	int nm_1 = 1;
	while (number_0[1] / 10 > 0)
	{
		nm_1++;
		number_0[1] = number_0[1] / 10;
	}
	for (int i = 0; i < 5 - nm_1; i++)
		format[nm_0 + i] = char(48);
	number_0[1] = number[1];
	for (int i = nm_0 + 4; i >= nm_0 + 5 - nm_1; i--)
	{
		format[i] = char(48 + number_0[1] % 10);
		number_0[1] = number_0[1] / 10;
	}
	format[nm_0 + 5] = '\0';
	delete[] number_0;
}
