package communication;

/**
 * @author Maciej Strozyk
 *
 */
public interface PacketConstants {
    

	/**
	 * Object types
	 */
	public static final short RSVP_SESSION_DST_ADDR = 1;
	public static final short RSVP_SESSION_SRC_ADDR = 2;
	public static final short AGENT_OUT_INTERFACE = 3;
	public static final short PATH_REFRESH_PERIOD = 4;
	public static final short TOKEN_BUCKET_RATE = 5;
	public static final short TOKEN_BUCKET_SIZE = 6;
	public static final short PEAK_DATA_RATE = 7;
	public static final short UPSTREAM_LABEL = 8 ;
	public static final short LSP_ENCODING_TYPE = 9;
	public static final short LSP_SWITCHING_TYPE = 10;
	public static final short LSP_GPID = 11;
	public static final short PATH_NAME = 12;
	public static final short ERO_ADDRESS = 13;
	public static final short ERO_PREFIX_LENGTH = 14;
	public static final short ERO_STRICT_LOOSE = 15;
	public static final short RSVP_HOP_ADDRESS = 16;
	public static final short ERROR_NODE_ADDRESS = 17;
	public static final short ERROR_FLAGS = 18;
	public static final short ERROR_CODE = 19;
	public static final short ERROR_VALUE = 20;
	public static final short MULTI_PART_MESAGE_PACKET_NUMBER = 21;
	public static final short MULTI_PART_MESAGE_TOTAL_PACKET_COUNT= 22;
	public static final short TUNNEL_ID = 23;
	public static final short TRAFFIC_DST_ADDR = 24;
	public static final short TRAFFIC_REVERSE_DST_ADDR = 25;
	public static final short DATA_INTERFACE_ID = 26;
	public static final short SOURCE_TNA = 27;
	public static final short DESTINATION_TNA = 28;
	public static final short CALL_ID_PART_1 = 29;	
	public static final short CALL_ID_PART_2 = 30;
	public static final short SENDER_LSP_ID = 31;
	public static final short SETUP_PRIORITY = 32;
	public static final short HOLD_PRIORITY = 33;
	public static final short PATH_STATE = 34; 
	public static final short PATH_SERV_LEVEL =  35;
	public static final short ETHERNET_CIR = 36; 
	public static final short ETHERNET_CBS = 37;
	public static final short ETHERNET_EIR = 38;
	public static final short ETHERNET_EBS = 39;
	public static final short ETHERNET_SW_GRANUALITY =  40;
	public static final short ETHERNET_INDEX = 41;
	public static final short ETHERNET_PROFILE = 42;

	
	/**
	 * Path states 
	 */
	public static final short PATH_STATUS_NULL = 0;
	public static final short PATH_STATUS_INITIATED = 1;
	public static final short PATH_STATUS_PATH_MESSAGE_SENT = 2;
	public static final short PATH_STATUS_UP = 6;
	public static final short PATH_STATUS_DOWN = 11;
	
	/**
	 * Data types
	 */
	public static final byte NO_TYPE = 0;
	public static final byte BYTE_TYPE = 1;
	public static final byte SHORT_TYPE = 2;
	public static final byte INT_TYPE = 3;
	public static final byte LONG_TYPE = 4;
	public static final byte FLOAT_TYPE = 5;
	public static final byte STRING_TYPE = 6;
	public static final byte VOID_TYPE = 7;
	
	public static final byte BYTE_TYPE_SIZE = 1;
	public static final byte SHORT_TYPE_SIZE = 2;
	public static final byte INT_TYPE_SIZE = 4;
	public static final byte LONG_TYPE_SIZE = 8;
	public static final byte FLOAT_TYPE_SIZE = 4;

	
	/**
	 * Header message types
	 */
	public static final byte REQ = 1;
	public static final byte ACK = 2;
	public static final byte NACK = 3;
	
	/**
	 * Header request types
	 * 
	 */
	public static final byte CREATE = 1;
	public static final byte DELETE = 2;
	public static final byte STATUS = 3;
	public static final byte TRANSIT = 4;
	

	

	//--------------------------------	
	public static final short PATH_REQUEST_TYPE = 0;
    public static final short PATH_REQUEST_ACKNACK_TYPE = 1;
    public static final short DELETE_REQUEST_TYPE = 2;
    public static final short DELETE_REQUEST_ACKNACK_TYPE = 3;

    
    public static final short SHORT_SIZE = 2;
    public static final short INT_SIZE = 4;
    public static final short LONG_SIZE = 8;
    
    public static final short NAME_SIZE = 32;

    //short+short+int+int++int+int+int+NAME_SIZE+LONG_SIZE
    public static final short PATH_REQUEST_PACKET_SIZE = 32 + NAME_SIZE;
    //short+short+int+short+short
    public static final short PATH_REQUEST_ACKNACK_PACKET_SIZE = 12;
    //short+short+int
    public static final short DELETE_REQUEST_PACKET_SIZE = 8;
    //short+short+short+short
    public static final short DELETE_REQUEST_ACKNACK_PACKET_SIZE = 8;
    
    public static final short RECEIVER_BUFFER_SIZE = 1024;
//-------------------------    

    /**
	 * Sizes of constants part of message
	 */
	
	public static final int  HEADER_SIZE = SHORT_TYPE_SIZE 
										 + 3 *BYTE_TYPE_SIZE;
	
	public static final int  OBJECT_HEADER_SIZE = SHORT_TYPE_SIZE 
	 									 + 2 *BYTE_TYPE_SIZE;
	
	public static final int  CONSTANT_OBJECT_PART_SIZE = SHORT_TYPE_SIZE 
	 												+ 2 *BYTE_TYPE_SIZE;

} // PacketConstants
