package com.example;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import javax.crypto.*;
import javax.xml.bind.DatatypeConverter;


public class DesSolution {
    public static void main(String[] args) throws Exception {
        String fileName = "C:\\Users\\It'sMine\\Desktop\\SUTD\\Term 5\\50.005  Computer System Engineering\\Labs\\Lab6\\shorttext.txt";
        String data = "";
        String line;
        BufferedReader bufferedReader = new BufferedReader( new FileReader(fileName));
        while((line= bufferedReader.readLine())!=null){
            data = data +"\n" + line;
        }
        System.out.println("Original content: "+ data);

//generate secret key using DES algorithm
        KeyGenerator keyGen = KeyGenerator.getInstance("DES");
        SecretKey desKey = keyGen.generateKey();
        
//create cipher object, initialize the ciphers with the given key, choose encryption mode as DES
        Cipher encryptCipher = Cipher.getInstance("DES/ECB/PKCS5Padding");   //"DES/EC   B/PKCS5Padding"
        encryptCipher.init(Cipher.ENCRYPT_MODE, desKey);

//do encryption, by calling method Cipher.doFinal().
        byte[] encryptresult = encryptCipher.doFinal(data.getBytes());
        String base64format = DatatypeConverter.printBase64Binary(encryptresult);
//        System.out.println(encryptresult.length);

//print the length of output encrypted byte[], compare the length of file smallSize.txt and largeSize.txt
//do format conversion. Turn the encrypted byte[] format into base64format String using DatatypeConverter
//print the encrypted message (in base64format String format)
        System.out.println("\n\nCipher text: " + base64format);
        System.out.println("Length of file: " + base64format.length());


//create cipher object, initialize the ciphers with the given key, choose decryption mode as DES
        Cipher decryptCipher = Cipher.getInstance("DES/ECB/PKCS5Padding");
        decryptCipher.init(Cipher.DECRYPT_MODE, desKey);

//do decryption, by calling method Cipher.doFinal().
        byte[] decryptresult = decryptCipher.doFinal(encryptresult);

//do format conversion. Convert the decrypted byte[] to String, using "String a = new String(byte_array);"
        String decryptedString = new String(decryptresult);

//print the decrypted String text and compare it with original text
        System.out.println("\nDecrypted text:" + decryptedString);
        if (decryptedString.compareTo(data) == 0){
            System.out.println("\nDecrypted text is same as the original text");
        }
    }
}