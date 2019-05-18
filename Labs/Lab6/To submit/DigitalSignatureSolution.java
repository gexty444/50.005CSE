package com.example;

import javax.xml.bind.DatatypeConverter;
import javax.crypto.Cipher;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.security.*;


public class DigitalSignatureSolution {

    public static void main(String[] args) throws Exception {
//Read the text file and save to String data
            String fileName = "C:\\Users\\It'sMine\\Desktop\\SUTD\\Term 5\\50.005  Computer System Engineering\\Labs\\Lab6\\shorttext.txt";
            String data = "";
            String line;
            BufferedReader bufferedReader = new BufferedReader( new FileReader(fileName));
            while((line= bufferedReader.readLine())!=null){
                data = data +"\n" + line;
            }
//            System.out.println("Original content: "+ data);

//generate a RSA keypair, initialize as 1024 bits, get public key and private key from this keypair.
            KeyPairGenerator keyGen = KeyPairGenerator.getInstance("RSA");
            keyGen.initialize(1024);
            KeyPair keyPair = keyGen.generateKeyPair();
            Key publicKey = keyPair.getPublic();
            Key privateKey = keyPair.getPrivate();

//Calculate message digest, using MD5 hash function
            MessageDigest md = MessageDigest.getInstance("MD5");
            byte[] digest = md.digest(data.getBytes());
            System.out.println("Decrypted digest as: " + DatatypeConverter.printBase64Binary(digest));


//print the length of output digest byte[], compare the length of file smallSize.txt and largeSize.txt
        System.out.println("\nLength of output digest byte[]: " + digest.length);
        System.out.println("Length of file: " + data.length());
           
//Create RSA("RSA/ECB/PKCS1Padding") cipher object and initialize is as encrypt mode, use PRIVATE key.
        Cipher cipher = Cipher.getInstance("RSA/ECB/PKCS1Padding");
        cipher.init(Cipher.ENCRYPT_MODE, privateKey);
//encrypt digest message
        byte[] encryptedBytes = cipher.doFinal(digest);

        System.out.println("encrypted bytes length:");
        System.out.println(encryptedBytes.length);


//print the encrypted message (in base64format String using DatatypeConverter)

        String base64format = DatatypeConverter.printBase64Binary(encryptedBytes);
        System.out.println("\nEncrypted message is: ");
        System.out.println(base64format);

//Create RSA("RSA/ECB/PKCS1Padding") cipher object and initialize is as decrypt mode, use PUBLIC key.
        cipher.init(Cipher.DECRYPT_MODE, publicKey);

//decrypt message
        byte[] decryptedBytes = cipher.doFinal(encryptedBytes);

//print the decrypted message (in base64format String using DatatypeConverter), compare with origin digest
        base64format = DatatypeConverter.printBase64Binary(decryptedBytes);
        System.out.println("\nDecrypted message is: ");
        System.out.println(base64format);


    }

}