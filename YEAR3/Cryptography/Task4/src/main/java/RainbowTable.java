/**
 * Rainbow table class
 * performs all operation on rainbow table data
 * @author andrew belcher
 */
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.io.Writer;
import static java.lang.Math.random;
import static java.lang.StrictMath.random;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import javax.swing.JFrame;
import javax.swing.JProgressBar;

/**
 *
 * @author wildcard
 */
public class RainbowTable {

    public String charset;
    public volatile int passlen;
    public volatile int chainlen;
    public volatile int numchains;
    public BigInteger mod;
    public HashMap<String, String> rain_table;
    public volatile int progress;
    public volatile int done;
    public String filename;
    public JProgressBar pbar;
    private static SecureRandom random;

    
    public RainbowTable(String charset, int passlen, int chainlen, int numchains) 
    {
        this.charset = charset;
        this.passlen = passlen;
        this.chainlen = chainlen;
        this.numchains = numchains;
        this.filename = "";
        this.mod = BigInteger.valueOf(0);
        this.done = 0;
        this.random = new SecureRandom();
    }
    
    /**
     * get prime number based on password length and charset
     * @return the prime
     */
    private BigInteger get_mod_prime()
    {
        BigInteger max = BigInteger.ZERO;

        for (int i = 1; i <= passlen; i++)
            max = max.add(BigInteger.valueOf(charset.length()).pow(i));

        BigInteger prime = max.nextProbablePrime();
        System.out.println("our prime: " + prime);
        
        return prime;
    }
    
    /**
    /**
     * converts byte array to hex string
     * @param data byte array of the raw data
     * @return the converted hex string
     */
    private static String convert_to_hex(byte[] data) 
    { 
        StringBuffer buf = new StringBuffer(); 
        
        for (int i = 0; i < data.length; i++) 
        { 
        	int halfbyte = (data[i] >>> 4) & 0x0F; 
        	int two_halfs = 0; 
        	do 
                { 
	            if ((0 <= halfbyte) && (halfbyte <= 9)) 
	                buf.append((char) ('0' + halfbyte)); 
	            else 
	            	buf.append((char) ('a' + (halfbyte - 10))); 
                    
	            halfbyte = data[i] & 0x0F; 
        	} 
                while(two_halfs++ < 1); 
        } 
        return buf.toString(); 
    } 
    
    /**
     * generate SHA-1 hash from string
     * @param text
     * @return 
     * @throws NoSuchAlgorithmException
     * @throws UnsupportedEncodingException 
     */
    public static String SHA1_hash(String text) throws NoSuchAlgorithmException, UnsupportedEncodingException  
    { 
	MessageDigest md; 
	md = MessageDigest.getInstance("SHA-1"); 
	byte[] sha1hash = new byte[40]; 
	md.update(text.getBytes("iso-8859-1"), 0, text.length()); 
	sha1hash = md.digest(); 
	return convert_to_hex(sha1hash); 
    } 
    
    /*
    private String reduce (BigInteger n) { // return an empty string if n <0 
      int base = charset.length();
          String alphanum = "abcdefghijklmnopqrstuvwxyz"+ "0123456789";

	BigInteger  r;
	//String s="";
        StringBuilder s = new StringBuilder();
	while(n.intValue()>0) {
		r = n.mod(BigInteger.valueOf(base)); 
		n = n.divide(BigInteger.valueOf(base)); // shift to right one digit (position) 
               // s = alphanum.charAt(r.intValue()) + s;
               s.append(charset.charAt(r.intValue()));
		n = n.subtract(BigInteger.valueOf(1));  // this line is special! need it if 
                                                        // we want handle different length of the strings 
	}
	return s.toString();
}*/
    
    /**
     * reduction function for producing plaintext from a hash
     * @param hash the hash string to be reduced
     * @param position interator to help reduce collisions
     * @return the reduced hash to plaintext
     */
    private String reduce(String hash, int position)
    {
        BigInteger index;
        StringBuilder builder = new StringBuilder();
        BigInteger temp = new BigInteger(hash, 16);

        // set position and modulo it with our prime
        temp = temp.add(BigInteger.valueOf(position));
        temp = temp.mod(this.mod);

        // build the reduced string using mod/set/divide
        while (temp.intValue() > 0)
        {
            index = temp.mod(BigInteger.valueOf(charset.length()));
            builder.append(charset.charAt(index.intValue()));
            temp = temp.divide(BigInteger.valueOf(charset.length()));
        }

        return builder.toString();
    }
    
    
    /**
     * generate a random string for the password
     * @param passlen the length of the password search space
     * @return the random password
     */
    private String gen_random_pass(int passlen)
    {
        // reinit random
        random = new SecureRandom();
        
        // catch if empty just in case
        if (passlen < 1) 
            throw new IllegalArgumentException();

        // build the password
        StringBuilder sb = new StringBuilder(passlen);
        for (int i = 0; i < passlen; i++) 
        {
            int rndCharAt = random.nextInt(charset.length());
            char rndChar = charset.charAt(rndCharAt);
            sb.append(rndChar);
        }
        return sb.toString();
    }
    

    /**
     * generate chain depending on the start password string
     * @param start the random password guess
     * @return the end of the chain
     * @throws NoSuchAlgorithmException
     * @throws UnsupportedEncodingException 
     */
    private String gen_chain(String start) throws NoSuchAlgorithmException, UnsupportedEncodingException
    {
        String password = start;
        
        for(int i=0; i < chainlen; i++)
            password = reduce(SHA1_hash(password),i);
        
        return password;
    }
    
   
    /**
     * generate the rainbow table
     * @return a hash map made of rows of 2 pairs of hash strings
     * @throws NoSuchAlgorithmException
     * @throws UnsupportedEncodingException 
     */ 
    public HashMap<String,String> gen_table() throws NoSuchAlgorithmException, UnsupportedEncodingException
    {
        // setup internal variables
        String start, key;
       // long start_time = System.nanoTime();
        int collisions = 0;
        BigInteger charlen = BigInteger.valueOf(charset.length());
        long inc = 0;        
        BigInteger test = BigInteger.valueOf(1);
        rain_table = new HashMap<String,String>(numchains);
        mod = get_mod_prime(); 
        
        // populate the table
        while(rain_table.size() < numchains)
        {
            // place holder since its probably wrong
            progress = (numchains / (rain_table.size()+1))%100;
            
            // setup chain
            start = gen_random_pass(passlen);
            key = gen_chain(start);
            
           // System.out.println("start: " + start + " key: " + key + " passlen: " + passlen + " chainlen: " + chainlen);

            //key exists so we will try again
            if(rain_table.containsKey(key))
                collisions++;
            
            // found a new key so store it and keep going till completion
            else
            {
                // update progress and add chain to the table
                pbar.setValue(progress);
                rain_table.put(key,start);
                inc = inc + numchains;
                System.out.println("start: " + start + 
                        " key: " + key + 
                        " passlen: " + passlen + 
                        " chainlen: " + chainlen +
                        " size: " + rain_table.size());
            }
        }
        
        return rain_table;
    }
    
    /**
     * write the rainbow table to a file
     * @param filename the filename
     * @param hash_map the generated table now in a hashmap
     */
    public void write_table(String filename, HashMap<String,String> hash_map)
    {
        String eol = System.getProperty("line.separator");
        System.out.println("numchains: " + numchains + " chainlen: " + chainlen + " passlen: " +  passlen);
        
        // setup header of the csv file
        try (Writer writer = new FileWriter(filename)) 
        {
            writer.append("magic:RBTL");
            writer.append(eol);

            writer.append("number of chains:");
            writer.append(Integer.toString(numchains));
            writer.append(eol);

            writer.append("length of chains:");            
            writer.append(Integer.toString(chainlen));
            writer.append(eol);
            
            writer.append("password length:");            
            writer.append(Integer.toString(passlen));
            writer.append(eol);
            
            writer.append("charset:");            
            writer.append(charset);
            writer.append(eol);
            
          // write the table to csv  
          for (Map.Entry<String, String> entry : hash_map.entrySet()) 
          {
            writer.append(entry.getKey())
                  .append(',')
                  .append(entry.getValue())
                  .append(eol);
          }
        } 
        catch (IOException ex) 
        {
          ex.printStackTrace(System.err);
        }
    }
    
    /**
     * read the table into the class hash map from a csv file
     * @param filename the filename 
     * @return 0:okay -1: format issue
     * @throws IOException 
     */
    public int read_table(String filename) throws IOException
    {
        rain_table = new HashMap<String,String>(numchains);
        BufferedReader csvReader = new BufferedReader(new FileReader(filename));
        String row;
        int i = 0;
        
        // for row in file
        while ((row = csvReader.readLine()) != null) 
        {
            // after header
            if(i>4)
            {
                String[] data = row.split(",");
                rain_table.put(data[0], data[1]);
               // System.out.println("start: " + data[0] + " key: " + data[1]);
            }
            // read in header and validate
            else
            {
                String[] data2 = row.split(":");
                System.out.println("start: " + data2[0] + " key: " + data2[1]);
                
                // check for magic
                if(i==0)
                    if(!data2[1].equals("RBTL"))
                        return -1;
                
                if(i==1)
                    numchains = Integer.parseInt(data2[1]);
                if(i==2)
                    chainlen = Integer.parseInt(data2[1]);
                if(i==3)
                    passlen = Integer.parseInt(data2[1]);
                if(i==4)
                    charset = data2[1];   
            }
            i++;
        }
        this.mod = get_mod_prime();
        System.out.println("numchains: " + numchains + " chainlen: " + chainlen + " passlen: " + passlen + "\ncharset: " + charset);
        csvReader.close();
    
        return 0;
    }
    
    /**
     * find the hash in the rainbow table
     * @param hash the target hash to find
     * @return the reversed password
     * @throws NoSuchAlgorithmException
     * @throws UnsupportedEncodingException 
     */
    public String find_in_table(String hash) throws NoSuchAlgorithmException, UnsupportedEncodingException
    {
        String target_hash = null;
        String pass = null;
        String found_pass = null;
        
        // walk the chain back from suspect hash
        for(int i= chainlen-1; i >= 0; i--)
        {
            target_hash = hash;
            
            // produce the chain to find the password
            for(int j=i; j<chainlen; j++)
            {
                pass = reduce(target_hash,j);
                target_hash = SHA1_hash(pass);
            }
            
            // found the password key in the table and target the chain
            if(rain_table.containsKey(pass))
            {
                found_pass = find_in_chain(rain_table.get(pass), hash);
                
                if(found_pass!=null)
                    break;
            }
        }
    
        return found_pass;
    }
    
    /**
     * find the hash in the chain based on the start and target hash
     * @param chain_start the start of the chain
     * @param hash the target hash
     * @return the password found
     * @throws NoSuchAlgorithmException
     * @throws UnsupportedEncodingException 
     */
    public String find_in_chain(String chain_start, String hash) throws NoSuchAlgorithmException, UnsupportedEncodingException
    {
        String target_hash = null;
        String pass = chain_start;
        String found_pass = null;
    
        // produce hash of start pass and start search
        for(int i = 0; i < chainlen; i++)
        {
            target_hash = SHA1_hash(pass);
            
            // found a match 
            if(target_hash.equals(hash))
            {
                found_pass = pass;
                break;
            }
            
            // else reduce again
            pass = reduce(target_hash, i);   
        }
    
        return found_pass;
    }
};
