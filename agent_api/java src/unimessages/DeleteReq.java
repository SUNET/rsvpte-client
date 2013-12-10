package unimessages;

import uniobjects.UniHeader;
/**
 * @author Maciej Strozyk
 *
 */

public class DeleteReq extends Message {
	
	/**
	 * 
	 * Constructor
	 *
	 * @param tunnelId
	 */
	public DeleteReq(short tunnelId) {
		
		super(UniHeader.REQ, UniHeader.DELETE);
		
		addTunnelId(tunnelId);
	
	}


} // DeleteReq
