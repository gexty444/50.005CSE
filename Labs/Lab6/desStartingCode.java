import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import javax.crypto.*;
import javax.xml.bind.DatatypeConverter;


public class DesSolution {
    public static void main(String[] args) throws Exception {
        String fileName = "smallSize.txt";
        String data = "";
        String line;
        BufferedReader bufferedReader = new BufferedReader( new FileReader(fileName));
        while((line= bufferedReader.readLine())!=null){
            data = data +"\n" + line;
        }
        System.out.println("Original content: "+ data);

//TODO: generate secret key using DES algorithm
        
//TODO: create cipher object, initialize the ciphers with the given key, choose encryption mode as DES

//TODO: do encryption, by calling method Cipher.doFinal().

//TODO: print the length of output encrypted byte[], compare the length of file smallSize.txt and largeSize.txt

//TODO: do format conversion. Turn the encrypted byte[] format into base64format String using DatatypeConverter

//TODO: print the encrypted message (in base64format String format)

//TODO: create cipher object, initialize the ciphers with the given key, choose decryption mode as DES

//TODO: do decryption, by calling method Cipher.doFinal().

//TODO: do format conversion. Convert the decrypted byte[] to String, using "String a = new String(byte_array);"

//TODO: print the decrypted String text and compare it with original text

    }
}