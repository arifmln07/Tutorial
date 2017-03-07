#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define MAXWORD 100

/* Fungsi prototipe */
void pause();
void inputChar(char *guessWord, char *wrongGuess, int *countGuess);
void secretInput(char *secretWord, char *hiddenWord, int *countWord );
void clear();
void hangaMan(int wrong);
void computeGuess(int *winner,
				  int *loser,
				  char *secretWord, 
				  char *guessWord, 
				  char *hiddenWord, 
				  char *wrongGuess, 
				  int *countWord, 
				  int *countW, 
				  int *countWrong);

int main()
{

	/* Inisialisasi nilai awal */
	char guessWord[30],wrongGuess[30], secretWord[MAXWORD], hiddenWord[MAXWORD];
	int countWrong =0, countW=0, countGuess = 0, countWord = 0, winner = 0, loser = 0;
	wrongGuess[0] = ' ';
	wrongGuess[1] = '\0';

    clear();	
    printf("\n\n\n     ##################################\n");
    printf("     #                                #\n");
    printf("     #        HANGMAN THE GAME        #\n");
    printf("     #                                #\n");
    printf("     ##################################\n \n");

    printf("      Enter the Secret Sentence : ");

	/* Input untuk memasukan kata yang akan ditebak pemain */
	secretInput(secretWord, hiddenWord, &countWord);
    clear();
    
    /* Looping hingga nilai winner atau loser menjadi 1 atau benar */
    while(1){
        printf("---------------------------------------------\n");
        if (!winner && !loser){		// Jika belum menang atau kalau, tampilkan game
                printf("Guess This =>  %s",hiddenWord);
        }
        if (winner){	// Jika menang, tampilkan ini
                clear();
                printf("\n\n\n\n\n\n            VICTORY! \n\n\n\n\n\n");
                pause();
                break;
        }
        if (loser){		// Jika kalah, tampilkan ini
                clear();
                printf("\n\n\n\n\n\n            YOU SHALL NOT PASS \n\n\n\n\n\n");
                pause();
                break;
        }
        printf("\n");
        printf("Wrong Guesses (%d) : %s",countWrong,wrongGuess);	// menampilkan huruf-huruf yang salah dan jumlahnya
        printf("\n");
        hangaMan(countWrong);		// menampilkan grafik hangman
        printf("\n");
        printf("\nGuess a character : ");
        inputChar(guessWord, wrongGuess, &countGuess);	// meminta input huruf yang akan ditebak
        computeGuess(&winner, &loser, secretWord, guessWord, hiddenWord, wrongGuess, &countWord, &countW, &countWrong); // fungsi untuk memproses semuanya
        clear();
    }
}

// fungsi pause
void pause(){

printf("\n\n   Press any key to continue... ");
getchar();
}

// fungsi clear screen
void clear(){
	system("clear");
}

// funsi untuk meminta input tebakan satu huruf
void inputChar(char *guessWord, char *wrongGuess, int *countGuess){
	
	int j;
	// meminta input dari buffer stdin hingga bertemu karakter enter
    for((*countGuess)=0; (guessWord[(*countGuess)] = tolower(getchar())) !='\n'; (*countGuess)++){ ;}
    
    guessWord[(*countGuess)]='\0';		// menandakan akhir dari string
    if ((*countGuess) > 1 || guessWord[0] == ' '){ 		// jika lebih dari satu kata atau memasukan karakter spasi maka lakukan

        printf("\nOnly Input 1 Character and not a Space. Guess a character : ");
        inputChar(guessWord, wrongGuess, countGuess); // rekursif hingga meng-input karakter yang benar
        clear();
    }
    for(j =0; j < 16; j++){	//

        if (wrongGuess[j] == guessWord[0]){	// mencek input apakah sudah pernah di input sebelumnya

        printf("\nYou have Guessed that. Guess another character : ");
        inputChar(guessWord, wrongGuess, countGuess); // jika sudah pernah maka rekursif hingga input karakter yang benar
        clear();
        break;
        }
    }
}

/* fungsi untuk memasukan kalimat yang akan ditebak */
void secretInput(char *secretWord, char *hiddenWord, int *countWord ){

	int i;
	// looping hingga mendapatkan karakter enter pada buffer stdin
    for((*countWord)=0; (secretWord[(*countWord)] = tolower(getchar()))!='\n'; (*countWord)++){ ;}
    secretWord[(*countWord)]='\0';	// menandakan akhir dari sebuah string
    if((*countWord) > MAXWORD ){	// lalukan jika input lebih dari huruf yang diizinkan
        clear();
        printf("\n\n\n     ##################################\n");
        printf("     #                                #\n");
        printf("     #        HANGMAN THE GAME        #\n");
        printf("     #                                #\n");
        printf("     ##################################\n \n");

        printf("      Please Input Sentence Below %d Character. \n      Enter the Secret Word : ", MAXWORD);
        secretInput(secretWord, hiddenWord, countWord );	// akan melakukan rekursif hingga kalimat yang diinput sesuai ketentuan
    }

	/* memasukan setiap kata ke string dimana semua katanya diganti menjadi ?, untuk pemain yang akan menebak */
    for(i = 0; i < (*countWord);i++){
        if(secretWord[i] != ' '){ hiddenWord[i] = '?';}
        else if(secretWord[i] == ' '){ hiddenWord[i] = ' ';}
    }
    hiddenWord[(*countWord)]='\0';	// menandakan akhir dari sebuah string

}

/* fungsi untuk menampilkan grafik hangman */
void hangaMan(int wrong){
	/* setiap kondisi jumlah kesalahan penebakan huruf memiliki rafiknya masing-masing */
    if (wrong == 0){printf("\n");}
    if (wrong == 1){
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
    }
    if (wrong == 2){
                printf("\n**********");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
    }
    if (wrong == 3){
                printf("\n**********");
                printf("\n*        *");
                printf("\n*        * * *");
                printf("\n*        **   *");
                printf("\n*        *****");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
    }
    if (wrong == 4){
                printf("\n**********");
                printf("\n*        *");
                printf("\n*        * * *");
                printf("\n*        **   *");
                printf("\n*        *****");
                printf("\n*           *");
                printf("\n*           *");
                printf("\n*           *");
                printf("\n*           *");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
    }
    if (wrong == 5){
                printf("\n**********");
                printf("\n*        *");
                printf("\n*        * * *");
                printf("\n*        **   *");
                printf("\n*        *****");
                printf("\n*         * *");
                printf("\n*        *  *");
                printf("\n*           *");
                printf("\n*           *");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
    }
    if (wrong == 6){
                printf("\n**********");
                printf("\n*        *");
                printf("\n*        * * *");
                printf("\n*        **   *");
                printf("\n*        *****");
                printf("\n*         * * *");
                printf("\n*        *  *  *");
                printf("\n*           *");
                printf("\n*           *");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
                printf("\n*");
    }
    if (wrong == 7){
                printf("\n**********");
                printf("\n*        *");
                printf("\n*        * * *");
                printf("\n*        **   *");
                printf("\n*        *****");
                printf("\n*         * * *");
                printf("\n*        *  *  *");
                printf("\n*           *");
                printf("\n*           *");
                printf("\n*          *");
                printf("\n*         *");
                printf("\n*");
                printf("\n*");
                printf("\n*");
    }

    if (wrong == 8){
                printf("\n**********");
                printf("\n*        *");
                printf("\n*        * * *");
                printf("\n*        **   *");
                printf("\n*        *****");
                printf("\n*         * * *");
                printf("\n*        *  *  *");
                printf("\n*           *");
                printf("\n*           *");
                printf("\n*          * *");
                printf("\n*         *   *");
                printf("\n*");
                printf("\n*");
                printf("\n*");
    }


}

/* Fungsi untuk mencek apakah inputan dari pemain merupakan tebakan yang salah atau benar */
void computeGuess(int *winner, 
				  int *loser, 
				  char *secretWord, 
				  char *guessWord, 
				  char *hiddenWord, 
				  char *wrongGuess, 
				  int *countWord, 
				  int *countW, 
				  int *countWrong){
		
	int i;			  
    int tempCorrect = 0;	
	int countCorrect = 0;
    for (i=0; i < (*countWord);i++){	
        if (secretWord[i] == guessWord[0] && secretWord[i] != ' '){
            hiddenWord[i] = secretWord[i];		// untuk mengubah huruf yang disembunyikan menjadi huruf yang benar jika tebakan benar
            tempCorrect +=1;		// menandakan bahwa tebak benar
        }
    }
    for (i=0; i < (*countWord);i++){
        if (secretWord[i] == hiddenWord[i]){	// men-cek jumlah tebakan yang benar
            countCorrect +=1;	// me-increment total huruf yang berhasil di tebak
        }
    }
    if (tempCorrect == 0){	// jika tebakan gagal maka lakukan
        wrongGuess[(*countW)] = guessWord[0];	// memasukan tebakan yang gagal ke string untuk ditampilkan ke pemain
        wrongGuess[(*countW)+1] = ' ';
        wrongGuess[(*countW)+2] = '\0';
        (*countW) += 2;		// untuk menempatkan huruf yang salah di posisi berikutnya
        (*countWrong) += 1;		// menaikan penhitung huruf yang salah
    }

    if (countCorrect == (*countWord)){ (*winner) = 1;}	// jika jumlah huruf yang benar samadengan huruf yang akan harus ditebak maka benar
    if ((*countWrong) == 8){(*loser) = 1;}		// jika sudah menebak salah sebanyak 8 kali maka gagal
}
