// This code takes as an input regular characters, encripts it into binary, and uses hexadecimal code in order to 
// develop a matrix QR out of 1's and 0's, correcpondig to 1's as white, and 0's as black.

#include "a2.h"

int bitwise_xor(int value){
  return (KEY ^ value);
}

char *xor_encrypt(char c){    //pointer to binary

    int decimal = bitwise_xor(c);
    int i;
    char *binary = malloc(sizeof(char)*8); // allocate space for a null terminating character as well

    for(i = 0; i < 7; i++){
      binary[i] = '0';
    } i = 0;
    // set the null terminating character
    binary[7]='\0';

    while(i < 7){
        if((decimal % 2) == 1){
            binary[6-i] = '1';
        }
        if ((decimal % 2)== 0){
            binary[6-i] = '0';
        }
        decimal /= 2;
        i++;
    }
    return binary;
}

char xor_decrypt(char *s){

    int power = 1;
    int decimal = 0;
  
    for(int i = 6; i >= 0; i--){
      if(s[i] == '1'){
        decimal += power;
      }
       power *=2;
    }

    decimal = (decimal ^ KEY);    
    return decimal;
}

char *gen_code(char *msg){

    int j, i = 0, l;
    int passed_terminate = 0;

    char * code = malloc(sizeof(char)*257);
    code[256] = '\0';

    //fill all the table to have a reference
    for(j = 0;j <= 255; j++){
        code[j] = '2';
    } j = 0;

    //fill with 0s  
    for(j = 0;j <= 255; j++){
        if(((j >= 17)&(j <= 19))||((j >= 28)&(j <= 30))||(j==33)||(j==35)||(j==44)||(j==46)||((j >= 49)&(j <= 51))||((j >= 60)&(j <= 62))||((j >= 192)&(j <= 194))||(j == 208)||(j == 210)||((j >= 225)&(j <= 227))){
        code[j] = '0';
        }
    } j = 0;

    //fill with 1s
    for(j = 0;j <= 255; j++){
        if((j <= 4)||((j >= 11)&(j <= 20)&(code[j] == '2'))||((j >= 27)&(j <= 36)&(code[j] == '2'))||((j >= 43)&(j <= 52)&(code[j] == '2'))||((j >= 59)&(j <= 68)&(code[j] == '2'))||((j >= 75)&(j <= 79))||((j >= 176)&(j <= 180)&(code[j] == '2'))||(j == 192)||(j == 196)||(j == 208)||(j == 210)||(j == 212)||(j == 224)||(j == 228)||((j >= 240)&(j <= 244))||(j == 255)){
        code[j] = '1';
        }
    } j = 0;

    passed_terminate = strlen(msg);
    
    // in the message, the final null-terminating character needs to be stored in the SC code - it cannot be assumed that
    // a null-terminating character will be encrypted (and thus stored) as an ASCII value of 0. A string of seven zeros (post encryption)
    // would indicate that the associated char (i.e. what you get when you decrypt the seven zeros) is equal to the KEY, which is not
    // necessarily \0. As such, you need to actually perform an encryption on the null-terminating character and store it in the SC code.
    // (also note, you need to store a null-terminating character at the end of the whole SC code, so there's actually 256 1s and 0s and then a 257th
    // \0). So, when the message gets decrypted, it will include the null-terminating character so that if that message is read it is guaranteed
    // to be a proper string in C.
    while(i <= passed_terminate){ //for every character in the message, i counts the letters

        char * en_msg = xor_encrypt(msg[i]); 
        for(int m = 0; m <= 6; m++){ 
            //here copies the string to the big array
            //en_msg = realloc(en_msg, sizeof(char)*(l+1));
            //l is taking the length of the entire message
            while (j < 256){
                if(code[j] == '2'){
                    code[j] = en_msg[m];
                    j=0;
                    break;
                }
                j++;
            } 
        }
        i++;
        free(en_msg);
    } i = 0; j = 0;
    
    //fill with code
    while(code[j] != '\0'){
        if(code[j] == '2'){
            code[j] = '0';
        }
        j++;
    } j = 0;
    //printf("free: %s\n", en_msg);
    //free(en_msg);
    /*
    while(j <= 255){
        if(code[j] == '2'){
            code[j] = '0';
        }
        j++;
    }*/
    return code;
}

char *read_code(char *code){
    //add code here
    /*
    This function deciphers the message encoded in an SC code and generates a string containing
    the deciphered message.
    When the SC code in Appendix A is passed to scan code() as a single string input, a pointer
    to the string "Program in C!" should be returned.
    Input: <code> is a pointer to a 256-digit string of ‘1’s and ‘0’s, representing an SC code
    containing an encrypted message.
    Output: Return a pointer to a string containing the message obtained by deciphering the
    input SC code.
    */
   char * en_str = malloc(sizeof(char)*181); 
   int i = 0;
   int j, l = 0;  

    // en_str[180] = '\0';
    // for(j = 0; j <= 179; j++){
    //     en_str[j] = '-';
    // }

    //fill the boxes and get the string
    for(j = 0;j <= 255; j++){
        if((j <= 4)||(j >= 11)&(j <= 20)||(j >= 27)&(j <= 36)||(j >= 43)&(j <= 52)||(j >= 59)&(j <= 68)||(j >= 75)&(j <= 79)||(j >= 176)&(j <= 180)||(j >= 192)&(j <= 196)||(j >= 208)&(j <= 212)||(j >= 224)&(j <= 228)||(j >= 240)&(j <= 244)||(j == 255)){
            code[j] = code[j];
        }
        else{
            en_str[i] = code[j];
            i++;
        }
    } i = 0; j = 0;

    char deciphered [8];
    char * final_str = malloc(sizeof(char)*27);
    final_str[26]= '\0';
    int m = 0;

    for(j = 0; j <= 26; j++){
        final_str[j] = '-';
    } j = 0;

    while(1){
        for(j = 0; j <= 6; j++){
            deciphered[j] = en_str[m + j];
        }
        deciphered[7] = '\0';
        final_str[l] = xor_decrypt(deciphered);

        if(final_str[l] == '\0'){
             break;
         }
        m += 7;
        l++;
    }

    free(en_str);
    return final_str;
    
}

char hexa_letter(int number){
    char value;
    if(number == 0){value = '0';}
    if(number == 1){value = '1';}
    if(number == 2){value = '2';}
    if(number == 3){value = '3';}
    if(number == 4){value = '4';}
    if(number == 5){value = '5';}
    if(number == 6){value = '6';}
    if(number == 7){value = '7';}
    if(number == 8){value = '8';}
    if(number == 9){value = '9';}

    if(number == 10){value = 'A';}
    if(number == 11){value = 'B';}
    if(number == 12){value = 'C';}
    if(number == 13){value = 'D';}
    if(number == 14){value = 'E';}
    if(number == 15){value = 'F';}

    return value;
}

char hexa_value(char *array){
    char hexa;
    int j, i=8, hexa_int=0;
    for(j = 0; j <= 3; j++){ //this will be returning the sum of the 4 char array
        if(array[j]=='1'){
            hexa_int+=i;
            i/=2;
        }
        else{
            i/=2;
        }
    }
    hexa = hexa_letter(hexa_int); //this will return the character of the 4 char array hopefully
    return hexa;
}

char *compress(char *code){
    /*
    This function generates a string representing the results of converting the binary digits of
    an SC code (‘1’s and ‘0’s) to hexadecimal digits (‘0’-‘9’, ‘A’-‘F’).
    When the SC code in Appendix A is passed to compress() as a single string input,
    the string "F83F8D11AAF58C71FA3F0B27DCDD3DE6439530000000F8008800A8008800F801"
    should be returned.
    Input: <code> is a pointer to a 256-digit string of ‘1’s and ‘0’s, representing an SC code.
    Output: Return a pointer to a 64-digit string containing the hexadecimal representation
    (‘0’-‘9’, ‘A’-‘F’) of the binary digits in <code>.
    */
    int j, m = 0;
    int l = 0;
    char * hexa_friend =malloc(sizeof(char)*6);
    hexa_friend[5] = '\0';
    char * hexa_val = malloc(sizeof(char)*65);
    hexa_val[64] = '\0';


    //for(int i = 0; i <= 62; i++){
    while(m <= 63){
        for(j = 0; j <= 4; j++){
            if((code[j+l] == '1')||(code[j+l] == '0')){
                hexa_friend[j] = code[j+l];
            }
        }   
        hexa_friend[5] = '0'; 
        hexa_val[m] = hexa_value(hexa_friend);
        
        m++;
        l += 4;
    }
    free(hexa_friend);
    return hexa_val;
}

char *decompress(char *code){
    //add code here
    /*
    This function generates a string representing the results of converting the hexadecimal digits
    (‘0’-‘9’, ‘A’-‘F’) of a compressed SC code to binary digits (‘1’s and ‘0’s).
    decompress("F83F8D11AAF58C71FA3F0B27DCDD3DE6439530000000F8008800A8008800F801")
    should return the SC code in Appendix A as a single 256-digit string of ‘1’s and ‘0’s.
    Input: <code> is a pointer to a 64-digit string containing the hexadecimal representation
    (‘0’-‘9’, ‘A’-‘F’) of a (compressed) SC code.
    Output: Return a pointer to a 256-digit string of ‘1’s and ‘0’s, representing an SC code.
    */
        //printf("\n\n\nhello");

    char * final_array = malloc(sizeof(char)*257);
    int j, finalint, dec, ASCII, l;
    final_array[256] = '\0';
    for(int m = 0; m<=255; m++){
        final_array[m] = '-';
    }

    for(j = 0; j<=63; j++){
        //first convert the caharcter to decimal value
        ASCII = code[j] - '\0';
        //check wether we have a decimal or A-F based on ASCII   
        if(ASCII <= 57){
            dec = code[j] -'0';
        }  
        else { //if letter, convert to exact binary form Hexa
            dec = code[j] - '7'; //decimal representation
        } 
        //printf("hex: %d\n", dec);
        for(l = ((4*j)) + 3; l >= 4*j; l--){
            finalint = dec % 2;
            final_array[l]= finalint+'0';
            dec = dec / 2;
        //printf("array: %c, %d, %c\n", finalint+'0', l, final_array[l]);
        }  
        // printf("final array: %s", final_array);
        // print_graph(final_array);
    }
    // print_graph(final_array);
    // printf("final array: %s", final_array);
    return final_array;
}

int calc_ld(char *sandy, char *cima){
    //add code here
    /*
    calculate the length of both strings with a for loop, the matrix will have those dimentions
    fill the first column from 0-lenght of each string -1
    start filling based on constraints form left to right at each row (ie: (y*, #))
    */
    int x=0, y=0, i=0, min=0;
    y = strlen(sandy);
    x = strlen(cima);

    //now we have 'matrix' dimensions
    int matrix[x+1][y+1];
    int col, row;

    //iterate through each row and column and check the cases
    for(col = 0; col <= y; col++){
        for(row = 0; row <= x; row++){
            //case 1 if S is an empty string
            if(col == 0){
                matrix[row][col] = row;
            }
            //case 2 if C is an empty string
            else if(row == 0){
                matrix[row][col] = col;
            }
            //case 3 S and C the same
            else if (sandy[col-1] == cima[row-1]){
                matrix[row][col] = matrix[row-1][col-1];
            }
            else{
                min = 0;
                if((matrix[row-1][col] < matrix[row-1][col-1])&&(matrix[row-1][col]<matrix[row][col-1])){
                    min = matrix[row-1][col];
                }
                else if(matrix[row][col-1]<matrix[row-1][col-1]){
                    min = matrix[row][col-1];
                }
                else{
                    min = matrix[row-1][col-1];
                }

                matrix[row][col] = min+1;
            }
        }
    }
    return matrix[x][y];   
}
