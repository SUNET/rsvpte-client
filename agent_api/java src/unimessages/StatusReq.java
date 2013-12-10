package unimessages;

import uniobjects.UniHeader;
/**
 * @author Maciej Strozyk
 *
 */

public class StatusReq extends Message {
	
	/**
	 * 
	 * Constructors
	 *
	 */
	public StatusReq() {
		
		super(UniHeader.REQ, UniHeader.STATUS);
	}
	

	public StatusReq(short tunnelId) {
		
		super(UniHeader.REQ, UniHeader.STATUS);

		addTunnelId(tunnelId);
	
	}


}//StatusReq
