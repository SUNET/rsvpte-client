package uniobjects;

import communication.PacketConstants;
/**
 * @author Maciej Strozyk
 *
 */


public abstract class UniObject implements PacketConstants {
	
	
	short objectType;
	byte dataType;
	byte dataSize;

	
	public byte generateDataSize(byte dataType) {

		switch(dataType) {
			case BYTE_TYPE : return BYTE_TYPE_SIZE;
			case SHORT_TYPE : return SHORT_TYPE_SIZE;
			case INT_TYPE : return INT_TYPE_SIZE;
			case LONG_TYPE : return LONG_TYPE_SIZE;
			case FLOAT_TYPE : return FLOAT_TYPE_SIZE;
			default: return 0;
		}
	}
	
	
	public byte generateDataType(short objectType) {
		
		switch(objectType) { 		
        	case RSVP_SESSION_DST_ADDR : return INT_TYPE;
        	case RSVP_SESSION_SRC_ADDR : return INT_TYPE;
        	case AGENT_OUT_INTERFACE : return INT_TYPE;
        	case PATH_REFRESH_PERIOD : return INT_TYPE;
        	case TOKEN_BUCKET_RATE : return FLOAT_TYPE;
        	case TOKEN_BUCKET_SIZE : return FLOAT_TYPE;
        	case PEAK_DATA_RATE : return FLOAT_TYPE;
        	//case UPSTREAM_LABEL : return VOID_TYPE;
        	case UPSTREAM_LABEL : return INT_TYPE;
        	case LSP_ENCODING_TYPE : return BYTE_TYPE;
        	case LSP_SWITCHING_TYPE : return BYTE_TYPE;
        	case LSP_GPID : return SHORT_TYPE;
        	case PATH_NAME : return STRING_TYPE;
        	case ERO_ADDRESS : return INT_TYPE;
        	case ERO_PREFIX_LENGTH : return BYTE_TYPE;
        	case ERO_STRICT_LOOSE : return BYTE_TYPE;
        	case RSVP_HOP_ADDRESS : return INT_TYPE;
        	case ERROR_NODE_ADDRESS : return INT_TYPE;
        	case ERROR_FLAGS : return BYTE_TYPE;
        	case ERROR_CODE : return BYTE_TYPE;
        	case ERROR_VALUE : return SHORT_TYPE;
        	case MULTI_PART_MESAGE_PACKET_NUMBER : return INT_TYPE;
        	case MULTI_PART_MESAGE_TOTAL_PACKET_COUNT : return INT_TYPE;
        	case TUNNEL_ID : return SHORT_TYPE;
        	case TRAFFIC_DST_ADDR : return INT_TYPE;
        	case TRAFFIC_REVERSE_DST_ADDR : return INT_TYPE;
        	case DATA_INTERFACE_ID : return INT_TYPE;
        	case SOURCE_TNA : return INT_TYPE;
        	case DESTINATION_TNA : return INT_TYPE;
        	case CALL_ID_PART_1 : return INT_TYPE;	
        	case CALL_ID_PART_2 : return INT_TYPE;
        	case SENDER_LSP_ID : return SHORT_TYPE;
        	case SETUP_PRIORITY : return BYTE_TYPE;
        	case HOLD_PRIORITY : return BYTE_TYPE;
        	case PATH_STATE : return SHORT_TYPE;
        	case PATH_SERV_LEVEL :return BYTE_TYPE;
        	case ETHERNET_CIR : return FLOAT_TYPE;
        	case ETHERNET_CBS : return FLOAT_TYPE;
        	case ETHERNET_EIR : return FLOAT_TYPE;
        	case ETHERNET_EBS : return FLOAT_TYPE;
        	case ETHERNET_SW_GRANUALITY : return SHORT_TYPE;
        	case ETHERNET_INDEX : return BYTE_TYPE;
        	case ETHERNET_PROFILE : return BYTE_TYPE;

        	default : return NO_TYPE;
		}
	}
	
	
	/**
	 * @return Returns the dataSize.
	 */
	public byte getDataSize() {
		return dataSize;
	}
	/**
	 * @param dataSize The dataSize to set.
	 */
	public void setDataSize(byte dataSize) {
		this.dataSize = dataSize;
	}
	/**
	 * @return Returns the dataType.
	 */
	public byte getDataType() {
		return dataType;
	}
	/**
	 * @param dataType The dataType to set.
	 */
	public void setDataType(byte dataType) {
		this.dataType = dataType;
	}
	/**
	 * @return Returns the objectType.
	 */
	public short getObjectType() {
		return objectType;
	}
	/**
	 * @param objectType The objectType to set.
	 */
	public void setObjectType(short objectType) {
		this.objectType = objectType;
	}

} //UniObject
