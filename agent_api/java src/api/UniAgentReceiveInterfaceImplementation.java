package api;

import unimessages.Message;
/**
 * @author Maciej Strozyk
 *
 */

public class UniAgentReceiveInterfaceImplementation implements
		UniAgentReceiveInterface {


	public void serviceReceivedMessage(Message receivedMessage) {
		
		//TODO message service ...
    	//implement any actions you need
    	
    	//message is displayed on screen for testing
		System.out.println("========== NEXT MESSAGE ==========\n");
		System.out.println(receivedMessage);
	}
	

} //UniAgentReceiveInterfaceImplementation
