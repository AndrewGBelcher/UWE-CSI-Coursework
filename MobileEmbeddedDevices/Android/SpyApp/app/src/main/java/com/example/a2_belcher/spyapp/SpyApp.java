// PACKAGES
package com.example.a2_belcher.spyapp;

// IMPORTS
import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
//import android.graphics.Camera;
import android.media.Image;
import android.media.MediaRecorder;
import android.os.Environment;
//import android.support.design.widget.Snackbar;
import android.support.constraint.ConstraintLayout;
import android.support.v4.app.ActivityCompat;
import android.support.v4.view.GestureDetectorCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Editable;
import android.text.Layout;
import android.text.TextWatcher;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.View;
import android.view.ViewConfiguration;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.StringWriter;
import java.security.*;
import java.util.Date;
import java.util.zip.DeflaterInputStream;
import java.util.zip.InflaterInputStream;
import javax.crypto.Cipher;
import javax.crypto.CipherInputStream;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import org.apache.commons.io.IOUtils;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.w3c.dom.Text;

import static javax.crypto.Cipher.DECRYPT_MODE;
import static javax.crypto.Cipher.ENCRYPT_MODE;
import android.view.View.OnTouchListener;
import android.support.v4.view.GestureDetectorCompat;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.View.OnClickListener;
// imports for UI components
import android.widget.Button;
import android.widget.EditText;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ImageButton;
// imports for Gesture Functionality
import android.support.v4.view.GestureDetectorCompat;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.MotionEvent;
import android.widget.Toast;
import android.view.View.OnTouchListener;
import android.content.Intent;
import java.util.Calendar;
import android.databinding.BaseObservable;
import android.databinding.Bindable;
import android.widget.Toolbar;


// OUR MAIN CLASS
public class SpyApp extends AppCompatActivity implements ActivityCompat.OnRequestPermissionsResultCallback {

    private android.hardware.Camera cameraObject;
    private ShowCamera showCamera;
    private ImageView pictureTaken;
    private MediaRecorder recorder;
    private String folder_main = "SpyApp";
    public static final String TAG = "MainActivity";
    private View mLayout;
    public boolean recording_running = false;
    public boolean recorder_open = false;
    int login_attempts = 3;
    public enum current_page{LOGIN, REGISTER, PASSCHANGE, CAMERA, FAKE};

    byte[] key;
    current_page cur_page;
    current_page prev_page;
    Cipher cipher;
    SecretKey skey;

    private static final int REQUEST_CAMERA = 0;
    private static final int REQUEST_CONTACTS = 1;
    private static final int REQUEST_EXTERNAL_STORAGE = 2;
    private static final int AUDIO_RECORD_REQUEST_CODE = 300;
    private static String[] PERMISSIONS_CONTACT = {Manifest.permission.READ_CONTACTS, Manifest.permission.WRITE_CONTACTS};

    String[] PERMISSIONS = {
            android.Manifest.permission.WRITE_EXTERNAL_STORAGE,
            android.Manifest.permission.RECORD_AUDIO,
            android.Manifest.permission.CAMERA
    };

    View mView = null;
    TextView et;
    Button imb;

    // CLASS METHODS
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        cur_page = current_page.LOGIN;
        prev_page = current_page.LOGIN;

        setContentView(R.layout.activity_spy_app_login);
        initCrypto();


        if(checkAppPermissions())
            requestAppPermissions();

        enableTextListenier();
    }


    public static void setCameraDisplayOrientation(Activity activity,
                                                   int cameraId, android.hardware.Camera camera) {

        android.hardware.Camera.CameraInfo info =
                new android.hardware.Camera.CameraInfo();

        android.hardware.Camera.getCameraInfo(cameraId, info);

        int rotation = activity.getWindowManager().getDefaultDisplay().getRotation();
        int degrees = 0;

        switch (rotation) {
            case Surface.ROTATION_0: degrees = 0; break;
            case Surface.ROTATION_90: degrees = 90; break;
            case Surface.ROTATION_180: degrees = 180; break;
            case Surface.ROTATION_270: degrees = 270; break;
        }

        int result;
        if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
            result = (info.orientation + degrees) % 360;
            result = (360 - result) % 360;  // compensate the mirror
        } else {  // back-facing
            result = (info.orientation - degrees + 360) % 360;
        }
        camera.setDisplayOrientation(result);
    }

    public void enableTextListenier()
    {

        Log.d(TAG,"in listener!");

        final EditText username_et = findViewById(R.id.login_username);
        final EditText password_et = findViewById(R.id.login_password);
        final Button regButton = findViewById(R.id.register_enter);

        //final boolean reg_btn_allow = false;


        username_et.addTextChangedListener(new TextWatcher() {

            @Override
            public void onTextChanged(CharSequence s, int start, int before,
                                      int count) {
                // TODO Auto-generated method stub

                if (s.toString().equals("") && password_et.getText().toString().equals("")) {
                    Log.d(TAG,"TEXT IS EMPTY!");
                   // reg_btn_allow = true;
                    regButton.setVisibility(View.VISIBLE);
                } else {
                    Log.d(TAG,"TEXT IS something!");
                    //reg_btn_allow = false;
                    regButton.setVisibility(View.INVISIBLE);
                }
            }

            @Override
            public void beforeTextChanged(CharSequence s, int start, int count,
                                          int after) {
                // TODO Auto-generated method stub

            }

            @Override
            public void afterTextChanged(Editable s) {
                // TODO Auto-generated method stub

            }
        });


        password_et.addTextChangedListener(new TextWatcher() {

            @Override
            public void onTextChanged(CharSequence s, int start, int before,
                                      int count) {
                // TODO Auto-generated method stub

                if (s.toString().equals("") && username_et.getText().toString().equals("")) {
                    Log.d(TAG,"TEXT IS EMPTY!");
                    // reg_btn_allow = true;
                    regButton.setVisibility(View.VISIBLE);
                } else {
                    Log.d(TAG,"TEXT IS something!");
                    //reg_btn_allow = false;
                    regButton.setVisibility(View.INVISIBLE);
                }
            }

            @Override
            public void beforeTextChanged(CharSequence s, int start, int count,
                                          int after) {
                // TODO Auto-generated method stub

            }

            @Override
            public void afterTextChanged(Editable s) {
                // TODO Auto-generated method stub

            }
        });


    }

    public Date lasttime;
    public int touchCount = 0;

    @Override
    public boolean onTouchEvent(MotionEvent event) {

        if(cur_page != current_page.FAKE) {
            super.onTouchEvent(event);

            Date currentTime = Calendar.getInstance().getTime();
            Log.d(TAG, "TOUCH!! cur time:" + currentTime + "\n\n");
            if (currentTime == currentTime)
                touchCount++;
            if (touchCount > 25) {
                Log.e(TAG, "\nyooooo\n\n");
                prev_page = cur_page;
                setContentView(R.layout.activity_spy_app_fake);
                cur_page = current_page.FAKE;



                ImageView image = findViewById(R.id.imageView3);

                image.setImageResource(R.drawable.screen);
                getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                        WindowManager.LayoutParams.FLAG_FULLSCREEN);
                getSupportActionBar().hide();

                image.getLayoutParams().height = ViewGroup.LayoutParams.MATCH_PARENT;
                image.getLayoutParams().width = ViewGroup.LayoutParams.MATCH_PARENT;
                image.setAdjustViewBounds(false);
                image.setScaleType(ImageView.ScaleType.FIT_XY);


                touchCount = 0;
            }
            lasttime = currentTime;
        }
        return false;
    }

    public void initCrypto ()
    {
        key = toByte("5F74FE7790127FECF82CC6E6D91FA2D1");// so bad here, we need to secure this later

        try
        {
            cipher = Cipher.getInstance("AES/CBC/PKCS5Padding", "BC");
            skey = new SecretKeySpec(key, 0, key.length, "AES");
        }
        catch (NoSuchPaddingException e)
        {
            Log.e(TAG, "no such padding: "+Log.getStackTraceString(e));
        }
        catch (NoSuchProviderException e)
        {
            Log.e(TAG, "no such provider: "+Log.getStackTraceString(e));
        }
        catch (NoSuchAlgorithmException e)
        {
            Log.e(TAG, "no such algo: "+Log.getStackTraceString(e));
        }
    }

    public static String byteArrayToHex(byte[] a)
    {
        StringBuilder sb = new StringBuilder(a.length * 2);
        for(byte b: a)
            sb.append(String.format("%02x", b));
        return sb.toString();
    }

    public static byte[] toByte(String hexString)
    {
        int len = hexString.length() / 2;
        byte[] result = new byte[len];
        for (int i = 0; i < len; i++)
        {
            result[i] = Integer.valueOf(hexString.substring(2 * i, 2 * i + 2), 16).byteValue();
        }
        return result;
    }




    public byte[] encrypt(byte[] data)
    {
        try
        {
            byte[] iv = new byte[16];
            cipher.init(ENCRYPT_MODE, skey, new IvParameterSpec(iv));
            DeflaterInputStream deflaterInput = new DeflaterInputStream(new CipherInputStream(new DeflaterInputStream(new ByteArrayInputStream(data)), cipher));
            Log.i(TAG,"iv:" + byteArrayToHex(iv));
            return IOUtils.toByteArray(deflaterInput);
        }

        catch (Exception e)
        {
            e.printStackTrace();
            return new byte[0];
        }
    }

    public byte[] decrypt(byte[] data)
    {
        byte[] result = new byte[0];

        try
        {
            byte[] iv = new byte[16];
            cipher.init(DECRYPT_MODE, skey, new IvParameterSpec(iv));

            InflaterInputStream inflaterStream = new InflaterInputStream(new CipherInputStream(new InflaterInputStream(new ByteArrayInputStream(data)), cipher));
            return IOUtils.toByteArray(inflaterStream);
        }
        catch (Exception e)
        {
            Log.e(TAG, "decryption out failed: "+Log.getStackTraceString(e));
            return result;
        }
    }

    public StringWriter writeXml(Document doc)
    {
        StringWriter writer = null;

        try
        {
            DOMSource domSource = new DOMSource(doc);
            writer = new StringWriter();
            StreamResult result = new StreamResult(writer);
            TransformerFactory tf = TransformerFactory.newInstance();
            Transformer transformer = tf.newTransformer();
            transformer.transform(domSource, result);
            System.out.println("XML IN String format is: " + writer.toString());
        }

        catch (javax.xml.transform.TransformerConfigurationException e)
        {
            e.printStackTrace();
        }

        catch (javax.xml.transform.TransformerException e)
        {
            e.printStackTrace();
        }

        return writer;
    }

    public int getUserCount(Document doc)
    {

        int user_count = 0;

            Element element = doc.getDocumentElement();
            element.normalize();

            NodeList nList = doc.getElementsByTagName("user");

            for (int i = 0; i < nList.getLength(); i++) {
                Node node = nList.item(i);
                if (node.getNodeType() == Node.ELEMENT_NODE) {
                    if (node.getNodeName().equals("user"))
                    {
                        user_count++;
                    }
                }
            }
        return user_count;
    }

    public boolean credentialsMatch(String username, String password, Document doc)
    {
        Element element = doc.getDocumentElement();
        boolean onUserEntry = false;

        element.normalize();

        NodeList nList = doc.getElementsByTagName("user");

        for (int i = 0; i < nList.getLength(); i++) {

            Node node = nList.item(i);
            if (node.getNodeType() == Node.ELEMENT_NODE) {
                if (node.getNodeName().equals("user")) {

                    Element element2 = (Element) node;
                    NodeList userCreds = element2.getChildNodes();

                    for (int d = 0; d < userCreds.getLength(); d++) {
                        Node credList1 = userCreds.item(d);
                        //Node credList2 = userCreds.item(d+);

                        if (credList1.getNodeType() == Node.ELEMENT_NODE) {
                            Element element3 = (Element) credList1;

                            if (credList1.getNodeName().equals("name")) {
                                if (username.equals(element3.getTextContent())) {
                                    onUserEntry = true;
                                }
                            }
                            if(credList1.getNodeName().equals("pass") && onUserEntry)
                            {
                                if(password.equals(element3.getTextContent())){
                                    Log.i(TAG, "pass: " + element3.getTextContent());
                                    return true;
                                }
                                onUserEntry = false;
                            }
                        }
                    }
                }
            }
        }
        return false;

    }

    public boolean usernameExists(String usernameChecked, Document doc)
    {
            Element element = doc.getDocumentElement();
            element.normalize();

            NodeList nList = doc.getElementsByTagName("user");

            for (int i = 0; i < nList.getLength(); i++) {

                Node node = nList.item(i);
                if (node.getNodeType() == Node.ELEMENT_NODE) {
                    if (node.getNodeName().equals("user")) {

                        Element element2 = (Element) node;
                        NodeList userCreds = element2.getChildNodes();

                        for (int d = 0; d < userCreds.getLength(); d++) {
                            Node credList = userCreds.item(d);

                            if (credList.getNodeType() == Node.ELEMENT_NODE) {
                                if (credList.getNodeName().equals("name")) {

                                    Element element3 = (Element) credList;

                                    if (usernameChecked.equals(element3.getTextContent())) {
                                        Log.i(TAG, "node: " + element3.getTextContent());
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        return false;
    }

    public InputStream readCredentialsFile()
    {
        File file = new File(Environment.getExternalStorageDirectory(), folder_main+"/credentials/details.sxml");
        int size = (int) file.length();
        byte[] sxml = null;
        byte[] xml = null;

        sxml = new byte[size];

        try
        {
            BufferedInputStream buf = new BufferedInputStream(new FileInputStream(file));
            buf.read(sxml, 0, sxml.length);
            buf.close();
        }

        catch (FileNotFoundException e)
        {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        catch (IOException e)
        {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        xml = decrypt(sxml);
        Log.i(TAG,"new decrypted data is: "+byteArrayToHex(xml));

        // String str = new String(dectest, "UTF-8"); // for UTF-8 encoding
        InputStream decxml = new ByteArrayInputStream(xml);
        Log.i(TAG, "new decrypted file is: " + decxml);

        return decxml;
    }

    public boolean writeCredentialsFile(String credFile)
    {
        // Make directory called RecordData if it doesn't already exist
        //File f = new File(Environment.getExternalStorageDirectory(), folder_main+"/credentials/");
        String path = Environment.getExternalStorageDirectory().toString();
        String filename = path + "/" + folder_main + "/" + "credentials/" + "details.sxml";

        byte[] xml = null;
        byte[] sxml = null;


        try
        {
            Log.i(TAG,"key is: "+ byteArrayToHex(key));
            Log.i(TAG,"skey is: "+ byteArrayToHex(skey.getEncoded()));
            Log.i(TAG,"key length: "+ key.length);

            xml = credFile.getBytes();
            Log.i(TAG,"decrypted data is: "+ byteArrayToHex(xml));
            sxml = encrypt(xml);
            Log.i(TAG,"encrypted data is: "+byteArrayToHex(sxml));
            xml = decrypt(sxml);
            Log.i(TAG,"new decrypted data is: "+byteArrayToHex(xml));
            String str = new String(xml, "UTF-8"); // for UTF-8 encoding
            Log.i(TAG,"new decrypted file is: "+str);
        }

        catch(java.lang.Exception e)
        {
            Log.e(TAG, "Exception: "+Log.getStackTraceString(e));
        }

        // enter the following code statement all on one line
        FileOutputStream outStream = null;

        try
        {
            outStream = new FileOutputStream(filename);
            outStream.write(sxml);
            outStream.close();
            return true;
        }
        catch (FileNotFoundException e)
        {
            e.printStackTrace();
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
        finally
        {
        }
        return false;
    }

    public Document getXmlDoc()
    {
        InputStream credFile = readCredentialsFile();

        Document doc = null;

        try{
            DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
            DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
            doc = dBuilder.parse(credFile);

            Element element = doc.getDocumentElement();
            element.normalize();
        }

        catch(org.xml.sax.SAXException e)
        {
            Log.e(TAG, "sax exception failed: " + Log.getStackTraceString(e));
        }
        catch(javax.xml.parsers.ParserConfigurationException e)
        {
            e.printStackTrace();
        }
        catch(java.io.IOException e)
        {
            e.printStackTrace();
        }
        return doc;

    }

    public void addNewUser(String username, String password, Document doc)
    {
        StringWriter writer = null;



                Element creds = (Element) doc.getElementsByTagName("credentials").item(0);

                Element newUser = doc.createElement("user");
                newUser.setAttribute("id", Integer.toString(getUserCount(doc) + 1));
                creds.appendChild(newUser);

                Element newName = doc.createElement("name");
                newName.setTextContent(username);
                newUser.appendChild(newName);

                Element newPass = doc.createElement("pass");
                newPass.setTextContent(password);
                newUser.appendChild(newPass);

                // transform doc back to xml string
                writer = writeXml(doc);

                if(writeCredentialsFile(writer.toString()));
                else
                    Log.i(TAG, "Error in writing encrypted file back!!");



    }


    public void changeUserPassword(String username, String password, Document doc)
    {
        StringWriter writer = null;

        Element element = doc.getDocumentElement();
        boolean onUserEntry = false;

        element.normalize();

        NodeList nList = doc.getElementsByTagName("user");

        Log.i(TAG, "in password change\n\n ");


        for (int i = 0; i < nList.getLength(); i++) {

            Node node = nList.item(i);
            if (node.getNodeType() == Node.ELEMENT_NODE) {
                if (node.getNodeName().equals("user")) {

                    Element element2 = (Element) node;
                    NodeList userCreds = element2.getChildNodes();

                    for (int d = 0; d < userCreds.getLength(); d++) {
                        Node credList1 = userCreds.item(d);
                        //Node credList2 = userCreds.item(d+);

                        if (credList1.getNodeType() == Node.ELEMENT_NODE)
                        {
                            Element element3 = (Element) credList1;

                            if (credList1.getNodeName().equals("name"))
                            {
                                if (username.equals(element3.getTextContent()))
                                {
                                    Log.i(TAG, "name matchs!: " + element3.getTextContent());
                                    onUserEntry = true;
                                }
                            }
                            if(credList1.getNodeName().equals("pass") && onUserEntry)
                            {
                                element3.setTextContent(password);
                                onUserEntry = false;
                            }

                        }
                    }
                }
            }
        }

        // transform doc back to xml string
        writer = writeXml(doc);

        if(writeCredentialsFile(writer.toString()));
        else
            Log.i(TAG, "Error in writing encrypted file back!!");

    }

    public void handleLoginAttempt(View view)
    {
        Log.i(TAG, "ON LOGIN CLICK attempts:" + login_attempts);

        EditText userText = (EditText)findViewById(R.id.login_username);
        EditText passText = (EditText)findViewById(R.id.login_password);
        String username = userText.getText().toString();
        String password = passText.getText().toString();



        // check if user exists!
        // check if password matchs
        if(checkAppPermissions()) {
            requestAppPermissions();
            userText.setText("");
            passText.setText("");
        }
        else {
            Document doc = getXmlDoc();

            // our credentials file
            File file = new File(Environment.getExternalStorageDirectory(), folder_main+"/credentials/details.sxml");
            if (file.exists())
            {

                if (username.equals("") || password.equals("")) {
                    if (username.equals(""))
                        Toast.makeText(getApplicationContext(), "Username cant be empty", Toast.LENGTH_SHORT).show();
                    else
                        Toast.makeText(getApplicationContext(), "Password cant be empty", Toast.LENGTH_SHORT).show();

                    userText.setText("");
                    passText.setText("");

                }

                else if (usernameExists(username, doc) && credentialsMatch(username, password, doc)) {
                    drawApp();
                }
                else {

                        TextView attempt_text = findViewById(R.id.attempts_view);
                        attempt_text.setVisibility(View.VISIBLE);
                        TextView attempt_num = findViewById(R.id.attempt_num);
                        attempt_num.setVisibility(View.VISIBLE);

                        login_attempts--;
                        attempt_num.setText(Integer.toString(login_attempts));

                        userText.setText("");
                        passText.setText("");

                        if (login_attempts == 0) {
                            attempt_num.setTextColor(Color.RED);
                            attempt_text.setTextColor(Color.RED);

                            Button btn = (Button) findViewById(R.id.login_confirm);
                            btn.setEnabled(false);
                        }
                    }
            }
            else
            {
                if (username.equals("") || password.equals("")) {
                    if (username.equals(""))
                        Toast.makeText(getApplicationContext(), "Username cant be empty", Toast.LENGTH_SHORT).show();
                    else
                        Toast.makeText(getApplicationContext(), "Password cant be empty", Toast.LENGTH_SHORT).show();
                }
                else {

                    Toast.makeText(getApplicationContext(), "User does not exist, please register!", Toast.LENGTH_SHORT).show();
                }
                userText.setText("");
                passText.setText("");
            }
        }
    }


    public void handleRegisterAttempt(View view) {

        EditText userText = (EditText)findViewById(R.id.reg_username);
        EditText passText = (EditText)findViewById(R.id.passchg_newpass);
        EditText passConfirmText = (EditText)findViewById(R.id.passchg_newpass_confirm);
        String username = userText.getText().toString();
        String password = passText.getText().toString();
        String passwordConfirm = passConfirmText.getText().toString();

        // gain access to filesystem write priv first
        if (checkAppPermissions()) {
            requestAppPermissions();
            userText.setText("");
            passText.setText("");
            passConfirmText.setText("");
        }

            // handle register attempt
        else
        {

            File filedir = new File(Environment.getExternalStorageDirectory(), folder_main+"/credentials");

            // our credentials file
            File file = new File(Environment.getExternalStorageDirectory(), folder_main+"/credentials/details.sxml");

            if (username.equals("") || password.equals("")) {

                if (username.equals(""))
                    Toast.makeText(getApplicationContext(), "Username cant be empty", Toast.LENGTH_SHORT).show();
                else if(password.equals(""))
                    Toast.makeText(getApplicationContext(), "Password cant be empty", Toast.LENGTH_SHORT).show();

                userText.setText("");
                passText.setText("");
                passConfirmText.setText("");
            }

            else if(password.equals(passwordConfirm)) {
                if (file.exists()) {

                    Document doc = getXmlDoc();

                    if (!usernameExists(username, doc))
                    {
                        addNewUser(username, password,doc);
                    }
                    else {
                        Toast.makeText(getApplicationContext(), "name is already taken", Toast.LENGTH_SHORT).show();
                    }
                }
                else {

                    filedir.mkdirs();

                    String templateCredData = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n" +
                            "<credentials>\n" +
                            "   <user id=" + '"' + "%d" + '"' + ">\n" +
                            "       <name>%s</name>\n" +
                            "       <pass>%s</pass>\n" +
                            "   </user>\n" +
                            "</credentials>";

                    templateCredData = String.format(templateCredData, 1, username, password);

                    if (writeCredentialsFile(templateCredData)) ;
                    else
                        Log.i(TAG, "Error in writing encrypted file back!!");

                }
                onBackPressed();
            }
            else
            {
                userText.setText("");
                passText.setText("");
                passConfirmText.setText("");

                Toast.makeText(getApplicationContext(), "Confirmation password does not match!", Toast.LENGTH_SHORT).show();
            }

        }
    }


    public void handlePassChangeAttempt(View view)
    {
        EditText userText = (EditText)findViewById(R.id.passchg_username);
        EditText oldPassText = (EditText)findViewById(R.id.passchg_prevpass);
        EditText newPassText = (EditText)findViewById(R.id.passchg_newpass);
        EditText newPassConfirmText = (EditText)findViewById(R.id.passchg_newpass_confirm);
        String username = userText.getText().toString();
        String oldPassword = oldPassText.getText().toString();
        String newPassword = newPassText.getText().toString();
        String newPasswordConfirm = newPassConfirmText.getText().toString();


        // gain access to filesystem write priv first
        if(checkAppPermissions()) {
            requestAppPermissions();

            userText.setText("");
            oldPassText.setText("");
            newPassText.setText("");
            newPassConfirmText.setText("");
        }
            // handle pass change attempt
        else
        {

            Document doc = getXmlDoc();

            if(usernameExists(username,doc) && credentialsMatch(username,oldPassword,doc))
            {
                if(newPassword.equals(newPasswordConfirm))
                {
                    changeUserPassword(username,newPassword,doc);
                    onBackPressed();
                }
                else
                {
                    userText.setText("");
                    oldPassText.setText("");
                    newPassText.setText("");
                    newPassConfirmText.setText("");

                    Toast.makeText(getApplicationContext(), "New password does not match confirmation", Toast.LENGTH_SHORT).show();
                }

            }
            else
            {
                if(!usernameExists(username,doc))
                    Toast.makeText(getApplicationContext(), "User does not exist", Toast.LENGTH_SHORT).show();
                else
                    Toast.makeText(getApplicationContext(), "Old password does not match", Toast.LENGTH_SHORT).show();

                userText.setText("");
                oldPassText.setText("");
                newPassText.setText("");
                newPassConfirmText.setText("");
            }

        }

    }

    public void handlePassChangeRequest(View view)
    {
        prev_page = current_page.LOGIN;
        setContentView(R.layout.activity_spy_app_passchange);
        cur_page = current_page.PASSCHANGE;
    }

    public void handleRegisterRequest(View view)
    {
        prev_page = current_page.LOGIN;
        setContentView(R.layout.activity_spy_app_register);
        cur_page = current_page.REGISTER;
    }




    public void drawApp()
    {
        prev_page = cur_page;
        cur_page = current_page.CAMERA;
        setContentView(R.layout.activity_spy_app);

        // carry on as normal
        pictureTaken = (ImageView) findViewById(R.id.imageView);
        cameraObject = isCameraAvailable();
        if(cameraObject!=null) {



            showCamera = new ShowCamera(this, cameraObject);
            FrameLayout preview = (FrameLayout) findViewById(R.id.camPreview);
            preview.addView(showCamera);

            Camera.Parameters parameters = cameraObject.getParameters();

          //  parameters.setPreviewSize(cameraObject.Size.width, mPreviewSize.height);
           // cameraObject.setParameters(parameters);
            cameraObject.setDisplayOrientation(90);
          //  cameraObject.setPreviewDisplay(preview);
            cameraObject.startPreview();

           // setCameraDisplayOrientation(SpyApp.this,0 ,cameraObject);

        }
    }

    public boolean hidden_ui = false;

    public void hide_ui(View view)
    {
        if(cur_page == current_page.CAMERA && !hidden_ui)
        {
            Button captureButton = findViewById(R.id.captureButton);
            Button recordButton = findViewById(R.id.recordButton);
            Button hide_ui_btn = findViewById(R.id.hide_ui_btn);
            FrameLayout cameraPreview = findViewById(R.id.camPreview);
            ImageView imgView = findViewById(R.id.imageView);
            imgView.setVisibility(View.INVISIBLE);
            cameraPreview.setVisibility(View.INVISIBLE);
            captureButton.setVisibility(View.INVISIBLE);
            recordButton.setVisibility(View.INVISIBLE);
            hide_ui_btn.setVisibility(View.INVISIBLE);
            getSupportActionBar().hide();

            hidden_ui = true;

        }
    }
    public void unhide_ui()
    {

            Button captureButton = findViewById(R.id.captureButton);
            Button recordButton = findViewById(R.id.recordButton);
            Button hide_ui_btn = findViewById(R.id.hide_ui_btn);
            FrameLayout cameraPreview = findViewById(R.id.camPreview);
            ImageView imgView = findViewById(R.id.imageView);
           // imgView.setVisibility(View.VISIBLE);
            cameraPreview.setVisibility(View.VISIBLE);
            captureButton.setVisibility(View.VISIBLE);
            recordButton.setVisibility(View.VISIBLE);
            hide_ui_btn.setVisibility(View.VISIBLE);
            getSupportActionBar().hide();

            hidden_ui = false;


    }

    @Override
    public void onBackPressed() {

        if(cur_page == current_page.LOGIN)
        {
            Intent intent = new Intent(Intent.ACTION_MAIN);
            intent.addCategory(Intent.CATEGORY_HOME);
            intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            startActivity(intent);
        }
        else if(cur_page == current_page.FAKE)
        {
            getSupportActionBar().show();

            if(prev_page == current_page.LOGIN) {
                setContentView(R.layout.activity_spy_app_login);
                cur_page = current_page.LOGIN;
                enableTextListenier();


            }
            else if(prev_page == current_page.CAMERA) {

                    drawApp();

            }
            else if(prev_page == current_page.REGISTER) {
                setContentView(R.layout.activity_spy_app_register);
                cur_page = current_page.REGISTER;
            }
            else
            {
                setContentView(R.layout.activity_spy_app_passchange);
                cur_page = current_page.PASSCHANGE;
            }
        }
        else
            {
                if(cur_page == current_page.CAMERA && hidden_ui)
                {
                    unhide_ui();
                }
                else {
                    login_attempts = 3;

                    cur_page = current_page.LOGIN;
                    setContentView(R.layout.activity_spy_app_login);

                    enableTextListenier();


                    TextView attempt_text = findViewById(R.id.attempts_view);
                    attempt_text.setVisibility(View.INVISIBLE);
                    TextView attempt_num = findViewById(R.id.attempt_num);
                    attempt_num.setVisibility(View.INVISIBLE);

                    Button btn = findViewById(R.id.login_confirm);
                    btn.setEnabled(true);
                }
            }

    }

    public static Camera isCameraAvailable()
    {
        Camera object = null;
        try {
            object = Camera.open();
        }

        catch (Exception e) {
        }
        return object;
    }

    public void backToCapturing(View view)
    {
        ImageView picture = findViewById(R.id.imageView);
        picture.setZ(0);
        picture.setVisibility(View.INVISIBLE);
        Button captureButton = findViewById(R.id.captureButton);
        Button recordButton = findViewById(R.id.recordButton);
        captureButton.setEnabled(true);
        recordButton.setEnabled(true);
        FrameLayout cameraPreview = findViewById(R.id.camPreview);
        cameraPreview.setVisibility(View.VISIBLE);
        captureButton.setVisibility(View.VISIBLE);
        recordButton.setVisibility(View.VISIBLE);
        ConstraintLayout background = findViewById(R.id.camBackground);
        background.setBackgroundColor(Color.WHITE);
    }

    private android.hardware.Camera.PictureCallback captureMedia = new Camera.PictureCallback()
    {

        @Override
        public void onPictureTaken(byte[] data, Camera camera) {
            Matrix matrix = new Matrix();
            matrix.postRotate(90F);
            Bitmap bitmap = BitmapFactory.decodeByteArray(data, 0, data.length);

            Bitmap newBitmap = Bitmap.createBitmap(bitmap,0, 0, bitmap.getWidth(), bitmap.getHeight(),   matrix, true);
            //imageView.setImageBitmap(newBitmap);
           // imageView.setBackgroundResource(imageBackground);

            if (bitmap == null) {
               // Toast.makeText(getApplicationContext(), "not taken", Toast.LENGTH_SHORT).show();
            } else {
               // Toast.makeText(getApplicationContext(), "taken", Toast.LENGTH_SHORT).show();
                pictureTaken.setImageBitmap(newBitmap);
                pictureTaken.setBackgroundResource(R.drawable.ic_launcher_background);
                pictureTaken.setZ(6);
                pictureTaken.setVisibility(View.VISIBLE);
                Button captureButton = findViewById(R.id.captureButton);
                Button recordButton = findViewById(R.id.recordButton);
                captureButton.setEnabled(false);
                recordButton.setEnabled(false);
                captureButton.setVisibility(View.INVISIBLE);
                recordButton.setVisibility(View.INVISIBLE);
                FrameLayout cameraPreview = findViewById(R.id.camPreview);
                cameraPreview.setVisibility(View.INVISIBLE);
                ConstraintLayout background = findViewById(R.id.camBackground);
                background.setBackgroundColor(Color.GRAY);




                // Make directory called RecordData if it doesn't already exist
                File f = new File(Environment.getExternalStorageDirectory(),
                        folder_main);
                if (!f.exists()) {
                    f.mkdirs();
                }
                String path = Environment.getExternalStorageDirectory().toString();

                // enter the following code statement all on one line
                String filename = path + "/" + folder_main + "/" + "recordData" + String.format("%d.jpg", System.currentTimeMillis());
                FileOutputStream outStream = null;
                try {
                    outStream = new FileOutputStream(filename);
                    outStream.write(data);
                    outStream.close();
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                } finally {
                }
            }
            ///cameraObject.release();
            cameraObject.startPreview();
        }
    };

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);

        // Checks the orientation of the screen
        if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE) {
            Toast.makeText(this, "landscape", Toast.LENGTH_SHORT).show();
        } else if (newConfig.orientation == Configuration.ORIENTATION_PORTRAIT){
            Toast.makeText(this, "portrait", Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        recorder.stop();
        recorder.reset();
        recorder.release();

        if (cameraObject != null) {
            cameraObject.stopPreview();
            cameraObject.release();
            cameraObject = null;
        }
    }

    public void captureImage(View view)
    {
        cameraObject.takePicture(null, null, captureMedia);
    }

    public void recordAudio(View view)
    {
        if(!recording_running)
        {

            // Make directory called RecordData if it doesn't already exist
            File f = new File(Environment.getExternalStorageDirectory(), folder_main);

            if (!f.exists()) {
                f.mkdirs();
            }

            String path = Environment.getExternalStorageDirectory().toString();


            // enter the following code statement all on one line
            String filename = path + "/" + folder_main + "/" + "recordData" + String.format("%d.3gp", System.currentTimeMillis());
            recorder = new MediaRecorder();
            recorder.setAudioSource(MediaRecorder.AudioSource.MIC);
            recorder.setOutputFormat(MediaRecorder.OutputFormat.THREE_GPP);
            recorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);
            recorder.setOutputFile(filename);

            try {
                recorder.prepare();
            } catch (IllegalStateException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
            recorder.start(); // Recording is now started
            recorder_open = true;
          //  Toast.makeText(getApplicationContext(), "recording..", Toast.LENGTH_SHORT).show();
        }

        else
            recorder.stop();

        recording_running = !recording_running;
    }

    private void requestAppPermissions()
    {
        Log.i(TAG, "CAMERA/Record audio/write storage permission has NOT been granted. Requesting permission.");
        ActivityCompat.requestPermissions(SpyApp.this, PERMISSIONS, 1);
        // END_INCLUDE(camera_permission_request)
    }

    private boolean checkAppPermissions()
    {
        return
                (
                // we dont have camera privs
                ActivityCompat.checkSelfPermission(this, Manifest.permission.CAMERA)
                != PackageManager.PERMISSION_GRANTED

                // we dont have audio privs
                || ActivityCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO)
                != PackageManager.PERMISSION_GRANTED

                // we dont have write storage privs
                || ActivityCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED);
    }

    class GestureHandler extends SimpleOnGestureListener
    {
        public boolean onFling(MotionEvent event1, MotionEvent event2, float velocityX, float velocityY)
        {
            Toast.makeText(SpyApp.this, "Purrrr", Toast.LENGTH_SHORT).show();
            return true;
        }

        public boolean onDoubleTap(MotionEvent e)
        {
            Intent intent;
            //intent = new Intent(SpyApp.this, ZoomImageActivity.class);
            //startActivity(intent);
            Toast.makeText(SpyApp.this, "Ouch!", Toast.LENGTH_SHORT).show();
            return true;
        }
    }
};


