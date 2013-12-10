package unimessages;

import uniobjects.UniHeader;
/**
 * @author Maciej Strozyk
 *
 */

public class DeleteNack extends Message {
	
	/**
	 * 
	 * Constructor
	 *
	 * @param tunnelId
	 */
	public DeleteNack(short tunnelId) {
		
		super(UniHeader.NACK, UniHeader.DELETE);
		
		addTunnelId(tunnelId);
	
	}

} //DeleteNack 
