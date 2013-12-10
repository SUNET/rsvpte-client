package unimessages;

import uniobjects.UniHeader;
/**
 * @author Maciej Strozyk
 *
 */

public class DeleteAck extends Message {

	/**
	 * 
	 * Constructor
	 *
	 * @param tunnelId
	 */
	public DeleteAck(short tunnelId) {
		
		super(UniHeader.ACK, UniHeader.DELETE);
		
		addTunnelId(tunnelId);
	
	}

}//DeleteAck 
