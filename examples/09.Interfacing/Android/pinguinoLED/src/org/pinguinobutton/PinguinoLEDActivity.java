package org.pinguinobutton;

import java.io.IOException;

import org.pinguinobutton.Server;
import org.pinguinobutton.AbstractServerListener;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class PinguinoLEDActivity extends Activity {
    /** Called when the activity is first created. */
	
	private Button Led1;
	private Button Led2;
	private Button Led3;
	private Button LightAll;
	private Button UnLightAll;	
	private Button Connect;
	private Button DisConnect;
	private TextView Tittle;
	
	boolean tcp_connected=false;
	
	Server tcpserver = null;
	
    @Override
    
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
            
        // create layout event
        
        this.Led1 = (Button)this.findViewById(R.id.Led1);
        this.Led2 = (Button)this.findViewById(R.id.Led2);
        this.Led3 = (Button)this.findViewById(R.id.Led3);
        this.LightAll = (Button)this.findViewById(R.id.LightAll);
        this.UnLightAll = (Button)this.findViewById(R.id.UnLightAll);       
        this.Connect = (Button)this.findViewById(R.id.Connect);
        this.DisConnect = (Button)this.findViewById(R.id.DisConnect);
        this.Tittle = (TextView)this.findViewById(R.id.Tittle);
        
        Led1.setOnClickListener(new View.OnClickListener() {
        	public void onClick(View v) { 
        		if (tcp_connected)
        			{
		        		try
		        		{
		        			tcpserver.send("S1");
		        			Tittle.setText("Led 1 is ON !!");		        			
		        		} catch (IOException e)
		        		{
		        			Log.e("pinguino", "problem sending TCP message", e);
		        		}	
        			}
        		else Tittle.setText("Start Server then connect Pinguino !!");
        	}
        });

        Led2.setOnClickListener(new View.OnClickListener() {
        	public void onClick(View v) {
        		if (tcp_connected)
    			{
	        		try
	        		{
	        			tcpserver.send("S2");
	        			Tittle.setText("Led 2 is ON !!");
	        		} catch (IOException e)
	        		{
	        			Log.e("pinguino", "problem sending TCP message", e);
	        		}	
    			}
        		else Tittle.setText("Start Server then connect Pinguino !!");
        	}
        });

        Led3.setOnClickListener(new View.OnClickListener() {
        	public void onClick(View v) { 
        		if (tcp_connected)
    			{
	        		try
	        		{
	        			tcpserver.send("S3");
	        			Tittle.setText("Led 3 is ON !!");
	        		} catch (IOException e)
	        		{
	        			Log.e("pinguino", "problem sending TCP message", e);
	        		}	        		
    			}
        		else Tittle.setText("Start Server then connect Pinguino !!");        		
        	}
        });

        LightAll.setOnClickListener(new View.OnClickListener() {
        	public void onClick(View v) {
        		if (tcp_connected)
        		{
	        		try
	        		{
	        			tcpserver.send("SA");
	        			Tittle.setText("All Led ON !!");
	        		} catch (IOException e)
	        		{
	        			Log.e("pinguino", "problem sending TCP message", e);
	        		}	        		        		
        		}
        		else Tittle.setText("Start Server then connect Pinguino !!");
        	}
        });

        UnLightAll.setOnClickListener(new View.OnClickListener() {
        	public void onClick(View v) { 
        		if (tcp_connected)
        		{
	        		try
	        		{
	        			tcpserver.send("RA");
	        			Tittle.setText("All Led Off !!");
	        		} catch (IOException e)
	        		{
	        			Log.e("pinguino", "problem sending TCP message", e);
	        		}	        		        		
        		}
        		else Tittle.setText("Start Server then connect Pinguino !!");
        	}
        });  
        
        Connect.setOnClickListener(new View.OnClickListener() {
        	public void onClick(View v) {
        		if (tcp_connected==false)
        		{
	        		try
	        		{
	        			tcpserver=new Server(2000);
	        			tcpserver.start();        			
	        			Tittle.setText("Server started !!");
	        			tcp_connected=true;
	        			
	        			tcpserver.addListener(new AbstractServerListener() {
	        				
	        				@Override
	        				public void onReceive(org.pinguinobutton.Client client, byte[] data)
	        				{
	        					String chaine="";
	        					
	        					for(int i=0; i<data.length; i++)
	        					       chaine = chaine + (char)data[i];

	        	        		try
	        	        		{
	        	        			tcpserver.send(chaine);
	        	        		} catch (IOException e)
	        	        		{
	        	        			Log.e("pinguino", "problem sending TCP message", e);
	        	        			return;
	        	        		}
	        					//Tittle.setText(chaine);

	        				};
	        				
	        			});	
	        			
	        		} catch (IOException e)
	        		{
	        		Tittle.setText("Pinguino is not connected !!");
	        		Log.e("pinguinoLed","unable to start server",e);
	        		tcp_connected=false;
	        		}
        		}
	        	else Tittle.setText("Already connected !!");
	        			
        		
        	}
        });  
        
        DisConnect.setOnClickListener(new View.OnClickListener() {
        	public void onClick(View v) {
        		tcpserver.stop();        			
				Tittle.setText("Server closed !!");
				tcp_connected=false;        		
        	}
        });          
    }
}