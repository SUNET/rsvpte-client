package communication;


import java.net.Inet4Address;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;

import unimessages.Message;
import uniobjects.UniByteObject;
import uniobjects.UniFloatObject;
import uniobjects.UniHeader;
import uniobjects.UniInetAddrObject;
import uniobjects.UniIntObject;
import uniobjects.UniObject;
import uniobjects.UniShortObject;
import uniobjects.UniStringObject;

/**
 * @author Maciej Strozyk
 *
 */
public class PacketUtils implements PacketConstants {
    
	
	// encode methods
	
	private static byte[] inet4AddressToByteArray(Inet4Address address) {
        byte[] array = new byte[INT_TYPE_SIZE];
        array = address.getAddress();
        return array;
    }
    

    private static byte[] shortToByteArray(short value) {
        
        byte[] array = new byte[SHORT_TYPE_SIZE];
        int index=SHORT_TYPE_SIZE-1;
        for(int i=0; i<SHORT_TYPE_SIZE; i++) 
            array[index--] = (byte)(value >> (i*8));
        return array;
    }
    
    private static byte[] intToByteArray(int value) {
        
        byte[] array = new byte[INT_TYPE_SIZE];
        int index=INT_TYPE_SIZE-1;
        for(int i=0; i<INT_TYPE_SIZE; i++)
            array[index--] = (byte)(value >> (i*8));
        return array;
    }

//    private static byte[] longToByteArray(long value) {
//        
//        byte[] array = new byte[LONG_TYPE_SIZE];
//        int index=LONG_TYPE_SIZE-1;
//        for(int i=0; i<LONG_TYPE_SIZE; i++)
//            array[index--] = (byte)(value >> (i*8));
//        return array;
//    }

    private static byte[] floatToByteArray(float value)
    {
    	byte[] array = new byte[FLOAT_TYPE_SIZE];
    	int intBits=Float.floatToIntBits(value);
        array[0]=(byte)((intBits&0x000000ff)>>0);
        array[1]=(byte)((intBits&0x0000ff00)>>8);
        array[2]=(byte)((intBits&0x00ff0000)>>16);
        array[3]=(byte)((intBits&0xff000000)>>24);
        
        return array;
    }
    
    
    
    /*
     * decode methods
     */
    
    public static float getFloatFromByteArray(byte[] array, int startIndex) {
		
		byte[] buffer = new byte[FLOAT_TYPE_SIZE];
		for (int i=0 ; i<FLOAT_TYPE_SIZE; i++)
			buffer[i] = array[startIndex+i];

		return ByteBuffer.wrap(buffer).order(ByteOrder.LITTLE_ENDIAN).getFloat();
	}
    
    
    static public short getShortFromByteArray(byte[] array, int startIndex) {
        
            byte[] buffer = new byte[SHORT_TYPE_SIZE];
            for ( int i=0; i<SHORT_TYPE_SIZE; i++)
                buffer[i] = array[startIndex+i];

            return ByteBuffer.wrap(buffer).order(ByteOrder.BIG_ENDIAN).getShort();
        }


    static public int getIntFromByteArray(byte[] array, int startIndex) {
    
        byte[] buffer = new byte[INT_TYPE_SIZE];
        for ( int i=0; i<INT_TYPE_SIZE; i++)
            buffer[i] = array[startIndex+i];
 
        return ByteBuffer.wrap(buffer).order(ByteOrder.BIG_ENDIAN).getInt();
    }

    
    static public long getLongFromByteArray(byte[] array, int startIndex) {
        
        byte[] buffer = new byte[LONG_TYPE_SIZE];
        for ( int i=0; i<LONG_TYPE_SIZE; i++)
            buffer[i] = array[startIndex+i];

        return ByteBuffer.wrap(buffer).order(ByteOrder.BIG_ENDIAN).getLong();

        }

    
    static public Inet4Address getInet4AddressFromByteArray(byte[] array, int startIndex) {
        
        byte addressArray[] = new byte[INT_TYPE_SIZE];
        Inet4Address i4address = null;
        for ( int i=0; i<INT_TYPE_SIZE; i++)
            addressArray[i] = array[startIndex+i];
        
        try {
            i4address = (Inet4Address) Inet4Address.getByAddress(addressArray);
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }
        return i4address;
    }

    
    static public String getStringFromByteArray(byte[] array, int startIndex, int length) {
        
        byte[] tmp = new byte[length];
        for (int i=0; i<length; i++)
            tmp[i] = array[startIndex+i];

        return new String(tmp);
     }
    
    
    static public boolean addArrayToArray(byte[] baseArray, int startIndex, byte[] newArray) {
        
    	if(startIndex+newArray.length>baseArray.length)
            return false;
        
        for(int i=0; i<newArray.length; i++)
            baseArray[startIndex+i] = newArray[i];

        return true;
    }

    //
    
    public static byte[] packPacketIntoArray(Message messagePacket) {
    	
    	int messageBufferSize = messagePacket.getMessageSize();

    	byte[] dataPacketBuf = new byte[messageBufferSize];
        int startIndex = 0;
        byte[] tmpBuf;
        UniHeader header = messagePacket.getHeader();
        
        // 
        tmpBuf = packHaeder(header);
        PacketUtils.addArrayToArray(dataPacketBuf,startIndex,tmpBuf);
        startIndex+=HEADER_SIZE;
        
        tmpBuf = packData(messagePacket.getUniObjects(),messageBufferSize-HEADER_SIZE);
        PacketUtils.addArrayToArray(dataPacketBuf,startIndex,tmpBuf);
    	
        return dataPacketBuf;
    }
    
    
    private static byte[] packData(List uniObjects, int dataBufferSize) {
    	
    	byte[] array = new byte[dataBufferSize];
    	byte[] tmpBuf = null;
    	byte[] objectHeader = new byte[OBJECT_HEADER_SIZE];
        int index=0;
        int offset=0;
        boolean tmpBufFull = false;

    	
    	Iterator objIterator = uniObjects.iterator();
    	while(objIterator.hasNext()) {
    		UniObject uniObj = (UniObject)objIterator.next();
    		offset = 0;
    		tmpBufFull = false;

    		tmpBuf = shortToByteArray(uniObj.getObjectType());
    		PacketUtils.addArrayToArray(objectHeader,0,tmpBuf);
    		objectHeader[2] = uniObj.getDataType();
    		objectHeader[3] = uniObj.getDataSize();
    		
    		PacketUtils.addArrayToArray(array,index,objectHeader);
    		index+=OBJECT_HEADER_SIZE;

    		switch(uniObj.getObjectType()) { 		
	        	case RSVP_SESSION_DST_ADDR : 
	        	case RSVP_SESSION_SRC_ADDR :
	        	case AGENT_OUT_INTERFACE :
	        	case RSVP_HOP_ADDRESS :
	        	case ERO_ADDRESS :
	        	case ERROR_NODE_ADDRESS :
	        	case TRAFFIC_DST_ADDR: 
	        	case TRAFFIC_REVERSE_DST_ADDR:
	        	case SOURCE_TNA:
	        	case DESTINATION_TNA: { 
	        		UniInetAddrObject obj = (UniInetAddrObject)uniObj; 
	        		tmpBuf = inet4AddressToByteArray(obj.getDataContent()); 
	        		offset = INT_TYPE_SIZE;
	        		tmpBufFull = true;
	        		break;}
	        	case PATH_REFRESH_PERIOD :
	        	case MULTI_PART_MESAGE_PACKET_NUMBER :
	        	case MULTI_PART_MESAGE_TOTAL_PACKET_COUNT : 
	        	case DATA_INTERFACE_ID :
	        	case CALL_ID_PART_1 :	
	        	case CALL_ID_PART_2 : 
	        	case UPSTREAM_LABEL :{
	        		UniIntObject obj = (UniIntObject)uniObj; 
	        		tmpBuf = intToByteArray(obj.getDataContent()); 
	        		offset = INT_TYPE_SIZE;
	        		tmpBufFull = true;
	        		break; }
	        	case TOKEN_BUCKET_RATE :
	        	case TOKEN_BUCKET_SIZE :
	        	case PEAK_DATA_RATE :
	        	case ETHERNET_CIR :
	        	case ETHERNET_CBS :
	        	case ETHERNET_EIR : 
	        	case ETHERNET_EBS :{
	        		UniFloatObject obj = (UniFloatObject)uniObj;
	        		tmpBuf = floatToByteArray(obj.getDataContent());
	        		offset = FLOAT_TYPE_SIZE;
	        		tmpBufFull = true;
	        		break; }

//	        	case UPSTREAM_LABEL :{
//	        		//TODO dopisac procedure;
//	        		offset = uniObj.getDataSize();
//	        		break;
//	        	}
	        	
	        	case LSP_ENCODING_TYPE : 
	        	case LSP_SWITCHING_TYPE :
	        	case ERO_PREFIX_LENGTH :
	        	case ERO_STRICT_LOOSE :
	        	case ERROR_FLAGS :
	        	case ERROR_CODE : 
	        	case SETUP_PRIORITY :
	        	case HOLD_PRIORITY : 
	        	case PATH_SERV_LEVEL :
	        	case ETHERNET_INDEX :
	        	case ETHERNET_PROFILE : {
	        		UniByteObject obj = (UniByteObject)uniObj;
	        		tmpBuf = new byte[1];
	        		tmpBuf[0] = obj.getDataContent(); 
	        		offset = BYTE_TYPE_SIZE;
	        		tmpBufFull = true;
	        		break; }
	        	case LSP_GPID :
	        	case ERROR_VALUE :
	        	case TUNNEL_ID : 
	        	case SENDER_LSP_ID :
	        	case PATH_STATE :
	        	case ETHERNET_SW_GRANUALITY : {
	        		UniShortObject obj = (UniShortObject)uniObj;
	        		tmpBuf = shortToByteArray(obj.getDataContent());
	        		offset = SHORT_TYPE_SIZE;
	        		tmpBufFull = true;
	        		break; }
	        	case PATH_NAME : {
	        		UniStringObject obj = (UniStringObject)uniObj;
	        		tmpBuf = obj.getDataContent().getBytes();
	        		offset = uniObj.getDataSize();
	        		tmpBufFull = true;
	        		break; }
    		}
    		if(tmpBufFull) {
	    		PacketUtils.addArrayToArray(array,index,tmpBuf);
	    		index+=offset;
    		}
    	}
		return array;
	}



	private static byte[] packHaeder(UniHeader header) {
    	
    	byte[] array = new byte[header.getHeaderSize()];
    	byte[] tmpBuf;
        int index=0;
        
    	//MessageId
        tmpBuf = shortToByteArray(header.getMessageId());
        PacketUtils.addArrayToArray(array,index,tmpBuf);
        index+=SHORT_TYPE_SIZE;
        
    	//MessageType
        array[index] = header.getMessageType();
        index+=BYTE_TYPE_SIZE;

    	//MessageRequest
        array[index] = header.getMessageRequest();
        index+=BYTE_TYPE_SIZE;

    	//MessageObjectsCounter
        array[index] = header.getMessageObjectsCounter();
        index+=BYTE_TYPE_SIZE;

    	return array;
	}

    
    
	
	public static Message unpackMessageArray(byte[] messageArray) {
    	
    	Message ietfMessage = null;
    	
        int index = 0;
        short messageId = getShortFromByteArray(messageArray,index);
        byte messageType = messageArray[index+=SHORT_TYPE_SIZE];
        byte messageRequest = messageArray[index+=BYTE_TYPE_SIZE];
        byte messageObjectsCounter = messageArray[index+=BYTE_TYPE_SIZE];
        index+= BYTE_TYPE_SIZE;
        
        ietfMessage = new Message(messageId, messageType, messageRequest);

        List l = unpackData(messageArray, index, messageObjectsCounter);
        ietfMessage.addObjectList(l);
        
        return ietfMessage;
    }
	
    
    private static List unpackData(byte[] messageArray, int index, byte messageObjectsCounter) {
		
    	List uniObjects = Collections.synchronizedList(new ArrayList());
    	
    	for(int i=0; i<messageObjectsCounter; i++){
			short objectType = getShortFromByteArray(messageArray,index);
			index+=SHORT_TYPE_SIZE;
			byte dataType = messageArray[index];
			index+=BYTE_TYPE_SIZE;
			byte dataSize = messageArray[index];
			index+=BYTE_TYPE_SIZE;

			switch(objectType) { 		
	        	case RSVP_SESSION_DST_ADDR :
	        	case RSVP_SESSION_SRC_ADDR :
	        	case AGENT_OUT_INTERFACE :
	        	case ERO_ADDRESS :
	        	case RSVP_HOP_ADDRESS :
	        	case ERROR_NODE_ADDRESS : 
	        	case TRAFFIC_DST_ADDR: 
	        	case TRAFFIC_REVERSE_DST_ADDR:
	        	case SOURCE_TNA:
	        	case DESTINATION_TNA: { 
	        		UniInetAddrObject obj = new UniInetAddrObject(objectType,
	        													  getInet4AddressFromByteArray(messageArray,index)); 
 
	        		uniObjects.add(obj);
	        		index+= INT_TYPE_SIZE;	
	        		break; }
	        	case PATH_REFRESH_PERIOD :
	        	case MULTI_PART_MESAGE_PACKET_NUMBER :
	        	case MULTI_PART_MESAGE_TOTAL_PACKET_COUNT : 
	        	case DATA_INTERFACE_ID :
	        	case CALL_ID_PART_1 :	
	        	case CALL_ID_PART_2 : 
	        	case UPSTREAM_LABEL : {
	        		UniIntObject obj = new UniIntObject(objectType,
	        											getIntFromByteArray(messageArray, index)); 
 
	        		uniObjects.add(obj);
	        		index+= INT_TYPE_SIZE;	
	        		break; }
	        	case TOKEN_BUCKET_RATE :
	        	case TOKEN_BUCKET_SIZE :
	        	case PEAK_DATA_RATE : 
	        	case ETHERNET_CIR :
	        	case ETHERNET_CBS :
	        	case ETHERNET_EIR : 
	        	case ETHERNET_EBS : {
	        		UniFloatObject obj = new UniFloatObject(objectType,
	        											 	getFloatFromByteArray(messageArray, index));
	        		uniObjects.add(obj);
	        		index+= FLOAT_TYPE_SIZE;
	        		break; }
	        	//case UPSTREAM_LABEL :{
	        		//TODO dopisac procedure;
	        		//index = uniObj.getDataSize();
	        		//break;
	        	//}
	        	
	        	case LSP_ENCODING_TYPE : 
	        	case LSP_SWITCHING_TYPE :
	        	case ERO_PREFIX_LENGTH :
	        	case ERO_STRICT_LOOSE :
	        	case ERROR_FLAGS :
	        	case ERROR_CODE : 
	        	case SETUP_PRIORITY :
	        	case HOLD_PRIORITY : 
	        	case PATH_SERV_LEVEL :
	        	case ETHERNET_INDEX :
	        	case ETHERNET_PROFILE : {
	        		UniByteObject obj = new UniByteObject(objectType,
	        											  messageArray[index]);
	        		uniObjects.add(obj);
	        		index+= BYTE_TYPE_SIZE;
	        		break;
	        	}
	        	case LSP_GPID :
	        	case ERROR_VALUE :
	        	case TUNNEL_ID : 
	        	case SENDER_LSP_ID :
	        	case PATH_STATE :
	        	case ETHERNET_SW_GRANUALITY : {
	        		UniShortObject obj = new UniShortObject(objectType,
	        												getShortFromByteArray(messageArray, index));
	        		uniObjects.add(obj);
	        		index+= SHORT_TYPE_SIZE;
	        		break;
	        	}
	        	case PATH_NAME : {
	        		UniStringObject obj = new UniStringObject(objectType,
	        												  getStringFromByteArray(messageArray,index,dataSize));
	        		uniObjects.add(obj);
	        		index+=dataSize;
	        		break;
	        	}
			}
		}
		
		return uniObjects;
	}


} // PacketUtils
