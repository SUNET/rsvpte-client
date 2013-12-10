package unimessages;

import uniobjects.UniHeader;
/**
 * @author Maciej Strozyk
 *
 */

public class CreateAck extends Message {

	/**
	 * 
	 * Constructor
	 *
	 * @param tunnelId
	 */
	public CreateAck(short tunnelId) {
		
		super(UniHeader.ACK, UniHeader.CREATE);

		addTunnelId(tunnelId);
	
	}	

} //CreateAck
