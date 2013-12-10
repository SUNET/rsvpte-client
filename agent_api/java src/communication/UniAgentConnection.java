package communication;

import java.io.IOException;
import java.net.InetAddress;
/**
 * @author Maciej Strozyk
 *
 */


public class UniAgentConnection {

	
	
	public CommunicationTCPSender communicationSender;
	CommunicationTCPReceiver communicationReceiver;
	boolean connected;
	
	
	/**
	 * 
	 * Constructor
	 *
	 * @param serverAddress
	 * @param serverPort
	 */
	public UniAgentConnection(InetAddress serverAddress, int serverPort) {
		
		connected = false;
		communicationSender = new CommunicationTCPSender(serverAddress, serverPort);
        
		if(communicationSender.getIn() != null) { 
			communicationReceiver = new CommunicationTCPReceiver(communicationSender.getIn());
			communicationReceiver.start();
            connected = true;
        }
	}

	
	public void closeConnection() {
        try {
        	if(communicationReceiver != null)
        		communicationReceiver.endReceiver();
            if(communicationSender != null) {
            	(communicationSender.getOut()).close();
            	(communicationSender.getIn()).close();
            	(communicationSender.getCommunicationSocket()).close();
            }
       } catch (IOException e1) {
           e1.printStackTrace();
       }
       connected = false;
	}


	/**
	 * @return Returns the communicationReceiver.
	 */
	public CommunicationTCPReceiver getCommunicationReceiver() {
		return communicationReceiver;
	}


	/**
	 * @return Returns the communicationSender.
	 */
	public CommunicationTCPSender getCommunicationSender() {
		return communicationSender;
	}


	/**
	 * @return Returns the connected.
	 */
	public boolean isConnected() {
		return connected;
	}

	
} // UniAgentConnection
