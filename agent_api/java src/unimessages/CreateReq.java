package unimessages;

import java.net.Inet4Address;
import uniobjects.UniHeader;
/**
 * @author Maciej Strozyk
 *
 */

public class CreateReq extends Message {
	
	/**
	 * 
	 * Constructor
	 *
	 * @param rsvpSessionDstAddr
	 * @param tunnelId
	 */
	public CreateReq(Inet4Address rsvpSessionDstAddr, short tunnelId) {
		
		super(UniHeader.REQ, UniHeader.CREATE);

		addRsvpSessionDstAddrObj(rsvpSessionDstAddr);
		addTunnelId(tunnelId);
	
	}


} //CreateReq
