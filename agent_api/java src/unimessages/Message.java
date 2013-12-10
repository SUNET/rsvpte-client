package unimessages;

import java.net.Inet4Address;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;
import java.util.Random;
import communication.PacketConstants;
import uniobjects.UniByteObject;
import uniobjects.UniFloatObject;
import uniobjects.UniHeader;
import uniobjects.UniInetAddrObject;
import uniobjects.UniIntObject;
import uniobjects.UniLongObject;
import uniobjects.UniObject;
import uniobjects.UniShortObject;
import uniobjects.UniStringObject;
import uniobjects.UniVoidObject;
/**
 * @author Maciej Strozyk
 *
 */

public class Message implements PacketConstants {
	
	
	protected UniHeader header;
	protected List uniObjects;
	

	/**
	 * 
	 * Constructor
	 *
	 * @param messageType
	 * @param messageRequest
	 */	
	public Message(byte messageType, byte messageRequest) {
		
		header = new UniHeader(getNewMessageId(),
							   messageType,
							   messageRequest);

		uniObjects = Collections.synchronizedList(new ArrayList());
	}	

	/**
	 * 
	 * Constructor
	 *
	 * @param messageId
	 * @param messageType
	 * @param messageRequest
	 */	
	public Message(short messageId, byte messageType, byte messageRequest) {
		
		header = new UniHeader(messageId,
							   messageType,
							   messageRequest);

		uniObjects = Collections.synchronizedList(new ArrayList());
	}	



	//	public Message(byte messageType, byte messageRequest) {
//		
//		header = new UniHeader(PacketUtils.getNewMessageId(),
//							   messageType,
//							   messageRequest);
//
//		uniObjects = Collections.synchronizedList(new ArrayList());
//	}	
	

	
	
	public short getNewMessageId() {
		Random r = new Random(); 
		return (short)(((r.nextInt() % Short.MAX_VALUE) + Short.MAX_VALUE) % Short.MAX_VALUE); 
		
	}
	/**
	 * @return Returns the header.
	 */
	public UniHeader getHeader() {
		return header;
	}

	/**
	 * @return Returns the uniObjects.
	 */
	public List getUniObjects() {
		return uniObjects;
	}
	
	
	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param dstAddress
	 * @return true if success
	 */
	public boolean addRsvpSessionDstAddrObj(Inet4Address dstAddress) {
		
		 boolean result = uniObjects.add(new UniInetAddrObject(RSVP_SESSION_DST_ADDR,
				 							  dstAddress)); 
		if(result)
			updateMessageObjectsCounter();
		
		return result; 
	}
	
	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param srcAddress
	 * @return
	 */
	public boolean addRsvpSessionSrcAddrObj(Inet4Address srcAddress) {
		
		boolean result = uniObjects.add(new UniInetAddrObject(RSVP_SESSION_SRC_ADDR,
											srcAddress));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param agentInterface
	 * @return true if success
	 */
	public boolean addAgentOutgoingInterface(Inet4Address agentInterface) {
		
		boolean result = uniObjects.add(new UniInetAddrObject(AGENT_OUT_INTERFACE,
														 agentInterface));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param pathRefreshPeriod
	 * @return true if success
	 */
	public boolean addPathRefreshPeriod(int pathRefreshPeriod) {
		
		boolean result = uniObjects.add(new UniIntObject(PATH_REFRESH_PERIOD,
														 pathRefreshPeriod));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param tokenBucketRate
	 * @return true if success
	 */
	public boolean addTokenBucketRate(float tokenBucketRate) {
		
		boolean result = uniObjects.add(new UniFloatObject(TOKEN_BUCKET_RATE,
														   tokenBucketRate));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param tokenBucketSize
	 * @return true if success
	 */
	public boolean addTokenBucketSize(float tokenBucketSize) {
		
		boolean result = uniObjects.add(new UniFloatObject(TOKEN_BUCKET_SIZE,
														   tokenBucketSize));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}
	
	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param peakDataRate
	 * @return true if success
	 */
	public boolean addPeakDataRate(float peakDataRate) {
		
		boolean result = uniObjects.add(new UniFloatObject(PEAK_DATA_RATE,
														   peakDataRate));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param upstreamLabel
	 * @return true if success
	 */
	public boolean addUpstreamLabel(int upstreamLabel) {
		
//		conversion from any long value into int value in binary representation
//		of unsigned int (4 bytes)

		// max value of unsigned int + 1 (2^32 = 4294967296)
		long unsignedIntMax = 4294967296l;
		long upstreamLabelLong;
		int upstreamLabelInt;
		
		if(upstreamLabel < 0)
		    upstreamLabelLong = upstreamLabel*(-1);
		else
			upstreamLabelLong = upstreamLabel;
			
		upstreamLabelLong = upstreamLabel % unsignedIntMax;

		if( upstreamLabelLong > Integer.MAX_VALUE)
		    upstreamLabelInt = (int)((upstreamLabelLong % Integer.MAX_VALUE)*(-1));
		else
		    upstreamLabelInt = (int)upstreamLabelLong;
 
		
		boolean result = uniObjects.add(new UniIntObject(UPSTREAM_LABEL,
														 upstreamLabelInt));
		if(result)
			updateMessageObjectsCounter();

		return result; 
	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param lspEncodingType
	 * @return true if success
	 */
	public boolean addLspEncodingType(byte lspEncodingType) {
		
		boolean result = uniObjects.add(new UniByteObject(LSP_ENCODING_TYPE,
														  lspEncodingType));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}
	
	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param lspSwitchingType
	 * @return true if success
	 */
	public boolean addLspSwitchingType(byte lspSwitchingType) {
		
		boolean result = uniObjects.add(new UniByteObject(LSP_SWITCHING_TYPE,
														  lspSwitchingType));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param lspGpid
	 * @return true if success
	 */
	public boolean addLspGpid(short lspGpid) {
		
		boolean result = uniObjects.add(new UniShortObject(LSP_GPID, lspGpid));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param pathName
	 * @return true if success
	 */
	public boolean addPathName(String pathName) {
		
		boolean result = uniObjects.add(new UniStringObject(PATH_NAME,
											pathName));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param eroAddress
	 * @return true if success
	 */
	public boolean addEroAddress(Inet4Address eroAddress) {
		
		boolean result = uniObjects.add(new UniInetAddrObject(ERO_ADDRESS,
											eroAddress));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param eroPrefixLength
	 * @return true if success
	 */
	public boolean addEroPrefixLength(byte eroPrefixLength) {
		
		boolean result = uniObjects.add(new UniByteObject(ERO_PREFIX_LENGTH,
														  eroPrefixLength));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param eroStrictLoose
	 * @return true if success
	 */
	public boolean addEroStrictLoose(byte eroStrictLoose) {
		
		boolean result = uniObjects.add(new UniByteObject(ERO_STRICT_LOOSE,
														  eroStrictLoose));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param rsvpHopAddress
	 * @return true if success
	 */
	public boolean addRsvpHopAddress(Inet4Address rsvpHopAddress) {
		
		boolean result = uniObjects.add(new UniInetAddrObject(RSVP_HOP_ADDRESS,
											rsvpHopAddress));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param errorNodeAddress
	 * @return true if success
	 */
	public boolean addErrorNodeAddress(Inet4Address errorNodeAddress) {
		
		boolean result = uniObjects.add(new UniInetAddrObject(ERROR_NODE_ADDRESS,
											errorNodeAddress));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param errorFlags
	 * @return true if success
	 */
	public boolean addErrorFlags(byte errorFlags) {
		
		boolean result = uniObjects.add(new UniByteObject(ERROR_FLAGS,
														  errorFlags));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param errorCode
	 * @return true if success
	 */
	public boolean addErrorCode(byte errorCode) {
		
		boolean result = uniObjects.add(new UniByteObject(ERROR_CODE,
														  errorCode));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param errorValue
	 * @return true if success
	 */
	public boolean addErrorValue(short errorValue) {
		
		boolean result = uniObjects.add(new UniShortObject(ERROR_VALUE,
														   errorValue));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param multipartMessagePacketNumber
	 * @return true if success
	 */
	public boolean addMultipartMessagePacketNumber(int multipartMessagePacketNumber) {
		
		boolean result = uniObjects.add(new UniIntObject(MULTI_PART_MESAGE_PACKET_NUMBER,
														 multipartMessagePacketNumber));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param multipartMessageTotalPacketCount
	 * @return true if success
	 */
	public boolean addMultipartMessageTotalPacketCount(int multipartMessageTotalPacketCount) {
		
		boolean result = uniObjects.add(new UniIntObject(MULTI_PART_MESAGE_TOTAL_PACKET_COUNT,
														 multipartMessageTotalPacketCount));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param tunnelId
	 * @return true if success
	 */
	public boolean addTunnelId(short tunneId) {
		
		boolean result = uniObjects.add(new UniShortObject(TUNNEL_ID,
														   tunneId));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param trafficDstAddress
	 * @return true if success
	 */
	public boolean addTrafficDstAddr(Inet4Address trafficDstAddress) {
		
		 boolean result = uniObjects.add(new UniInetAddrObject(TRAFFIC_DST_ADDR,
				 											   trafficDstAddress)); 
		if(result)
			updateMessageObjectsCounter();
		
		return result; 
	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param trafficRevDstAddress
	 * @return true if success
	 */
	public boolean addTrafficReverseDstAddr(Inet4Address trafficRevDstAddress) {
		
		 boolean result = uniObjects.add(new UniInetAddrObject(TRAFFIC_REVERSE_DST_ADDR,
				 											   trafficRevDstAddress)); 
		if(result)
			updateMessageObjectsCounter();
		
		return result; 
	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param dataInterfaceId
	 * @return true if success
	 */
	public boolean addDataInterfaceId(int dataInterfaceId) {
		
		boolean result = uniObjects.add(new UniIntObject(DATA_INTERFACE_ID,
														 dataInterfaceId));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}
	
	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param sourceTNAAddress
	 * @return true if success
	 */
	public boolean addSourceTNA(Inet4Address sourceTNAAddress) {
		
		 boolean result = uniObjects.add(new UniInetAddrObject(SOURCE_TNA,
				 											   sourceTNAAddress)); 
		if(result)
			updateMessageObjectsCounter();
		
		return result; 
	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param destinationTNAAddress
	 * @return true if success
	 */
	public boolean addDestinationTNA(Inet4Address destinationTNAAddress) {
		
		 boolean result = uniObjects.add(new UniInetAddrObject(DESTINATION_TNA,
				 											   destinationTNAAddress)); 
		if(result)
			updateMessageObjectsCounter();
		
		return result; 
	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param callIdPartOne
	 * @return true if success
	 */
	public boolean addCallIdPartOne(int callIdPartOne) {
		
		boolean result = uniObjects.add(new UniIntObject(CALL_ID_PART_1,
														 callIdPartOne));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 
	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param callIdPartTwo
	 * @return true if success
	 */
	public boolean addCallIdPartTwo(int callIdPartTwo) {
		
		boolean result = uniObjects.add(new UniIntObject(CALL_ID_PART_2,
														 callIdPartTwo));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param senderLSPId
	 * @return true if success
	 */
	public boolean addSenderLSPId(short senderLSPId) {
		
		boolean result = uniObjects.add(new UniShortObject(SENDER_LSP_ID,
														 senderLSPId));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}
	
	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param setupPriority
	 * @return true if success
	 */
	public boolean addSetupPriority(byte setupPriority) {
		
		boolean result = uniObjects.add(new UniByteObject(SETUP_PRIORITY,
														  setupPriority));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param holdPriority
	 * @return true if success
	 */
	public boolean addHoldPriority(byte holdPriority) {
		
		boolean result = uniObjects.add(new UniByteObject(HOLD_PRIORITY,
														  holdPriority));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param pathStatus
	 * @return true if success
	 */
	public boolean addPathStatus(short pathStatus) {
		
		boolean result = uniObjects.add(new UniShortObject(PATH_STATE, pathStatus));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}
	
	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param pathServLevel
	 * @return true if success
	 */
	public boolean addPathServLevel(byte pathServLevel) {
		
		boolean result = uniObjects.add(new UniByteObject(PATH_SERV_LEVEL, pathServLevel));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param ethCir
	 * @return true if success
	 */
	public boolean addEthCir(float ethCir) {
		
		boolean result = uniObjects.add(new UniFloatObject(ETHERNET_CIR, ethCir));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}
	
	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param ethCbs
	 * @return true if success
	 */
	public boolean addEthCbs(float ethCbs) {
		
		boolean result = uniObjects.add(new UniFloatObject(ETHERNET_CBS, ethCbs));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}
	
	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param ethEir
	 * @return true if success
	 */
	public boolean addEthEir(float ethEir) {
		
		boolean result = uniObjects.add(new UniFloatObject(ETHERNET_EIR, ethEir));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}
	
	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param ethEbs
	 * @return true if success
	 */
	public boolean addEthEbs(float ethEbs) {
		
		boolean result = uniObjects.add(new UniFloatObject(ETHERNET_EBS, ethEbs));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param ethSwGranuality
	 * @return true if success
	 */
	public boolean addEthSwGranuality(short ethSwGranualit) {
		
		boolean result = uniObjects.add(new UniShortObject(ETHERNET_SW_GRANUALITY, ethSwGranualit));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param ethIndex
	 * @return true if success
	 */
	public boolean addEthIndex(byte ethIndex) {
		
		boolean result = uniObjects.add(new UniByteObject(ETHERNET_INDEX, ethIndex));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	/**
	 * Adds new object to message objects list. 
	 * 
	 * @param ethProfile
	 * @return true if success
	 */
	public boolean addEthProfile(byte ethProfile) {
		
		boolean result = uniObjects.add(new UniByteObject(ETHERNET_PROFILE, ethProfile));
		if(result)
			updateMessageObjectsCounter();
		
		return result; 

	}

	public void addObjectList(List objectList){
		uniObjects = objectList;
		updateMessageObjectsCounter();
	}
	
	
	/**
	 * Returns size of message in bytes
	 * @return
	 */
	public int getMessageSize() {
		int messageSize = 0;
		UniObject uniObj;
		messageSize = HEADER_SIZE;
		Iterator i = uniObjects.iterator();
		while(i.hasNext()) {
			uniObj = (UniObject)i.next();
			messageSize+=CONSTANT_OBJECT_PART_SIZE;
			messageSize+=getSizeOfObjectData(uniObj);
		}
		
		return messageSize;
	}
	

	public void printMessage() {
        
		System.out.println("---Message HEADER---");
		System.out.println("Message id:"+ header.getMessageId());
		System.out.println("Message type:"+ header.getMessageType());
		System.out.println("Message request type:"+ header.getMessageRequest());
		System.out.println("Message objects number:"+ header.getMessageObjectsCounter());
		System.out.println("---Message OBJECTS---");
		Iterator i = uniObjects.iterator();
		while(i.hasNext()) {
			UniObject o = (UniObject)i.next();
			System.out.println("\n-----Message object");
			System.out.println("Object type:"+ o.getObjectType());
			System.out.println("Object data type:"+ o.getDataType());
			System.out.println("Object data size:"+ o.getDataSize());
			
			switch(o.getDataType()) {
				case BYTE_TYPE : { 
					UniByteObject bo = (UniByteObject)o;
					System.out.println("Data content: "+bo.getDataContent());
					break; }
				case SHORT_TYPE : {
					UniShortObject so = (UniShortObject)o;
					System.out.println("Data content: "+so.getDataContent());
					break; }
				case INT_TYPE : {
					short type = o.getObjectType();
					if(type==RSVP_SESSION_DST_ADDR ||
					   type==RSVP_SESSION_SRC_ADDR ||
					   type==AGENT_OUT_INTERFACE ||
					   type==ERO_ADDRESS ||
					   type==RSVP_HOP_ADDRESS ||
					   type==ERROR_NODE_ADDRESS ||
					   type==TRAFFIC_DST_ADDR ||
					   type==TRAFFIC_REVERSE_DST_ADDR ||
					   type==SOURCE_TNA ||
					   type==DESTINATION_TNA){

						UniInetAddrObject io = (UniInetAddrObject)o;
					   System.out.println("Data content: "+(io.getDataContent()).getHostAddress());
					}
					else {
						UniIntObject io = (UniIntObject)o;
						System.out.println("Data content: "+io.getDataContent());
					}
					break; }
				case LONG_TYPE : {
					UniLongObject lo = (UniLongObject)o;
					System.out.println("Data content: "+lo.getDataContent());
					break; }
				case FLOAT_TYPE : {
					UniFloatObject fo = (UniFloatObject)o;
					System.out.println("Data content: "+fo.getDataContent());
					break; }
				case STRING_TYPE : {
					UniStringObject so = (UniStringObject)o;
					System.out.println("Data content: "+so.getDataContent());
					break; }
				case VOID_TYPE : {
					UniVoidObject so = (UniVoidObject)o;
					System.out.println("Data content: "+so.getDataContent());
					break; }
				default: System.out.println("-ERROR-Unknown data type!");
			}
			System.out.println("-----Message object");
		}
        
    }

    public String toString() {
        String str = new String();
		
        str = "-----Message HEADER-----"
        	+"\nMessage id:"+ header.getMessageId()
        	+"\nMessage type:"+ header.getMessageType()
        	+"\nMessage request type:"+ header.getMessageRequest()
        	+"\nMessage objects number:"+ header.getMessageObjectsCounter()
        	+"\n-----Message OBJECTS-----";
		Iterator i = uniObjects.iterator();
		while(i.hasNext()) {
			UniObject o = (UniObject)i.next();
			str+= "\n-----Message object"
				+"\nObject type:"+ o.getObjectType()
				+"\nObject data type:"+ o.getDataType()
				+"\nObject data size:"+ o.getDataSize();
			switch(o.getDataType()) {
				case BYTE_TYPE : { 
					UniByteObject bo = (UniByteObject)o;
					str+="\nData content:"+bo.getDataContent();
					break; }
				case SHORT_TYPE : {
					UniShortObject so = (UniShortObject)o;
					str+="\nData content:"+so.getDataContent();
					break; }
				case INT_TYPE : {
					short type = o.getObjectType();
					if(type==RSVP_SESSION_DST_ADDR ||
					   type==RSVP_SESSION_SRC_ADDR ||
					   type==AGENT_OUT_INTERFACE ||
					   type==ERO_ADDRESS ||
					   type==RSVP_HOP_ADDRESS ||
					   type==ERROR_NODE_ADDRESS ||
					   type==TRAFFIC_DST_ADDR ||
					   type==TRAFFIC_REVERSE_DST_ADDR ||
					   type==SOURCE_TNA ||
					   type==DESTINATION_TNA){

						UniInetAddrObject io = (UniInetAddrObject)o;
						str+="\nData content:"+(io.getDataContent()).getHostAddress();
					}
					else {
						UniIntObject io = (UniIntObject)o;
						str+="\nData content:"+io.getDataContent();
					}
					break; }
				case LONG_TYPE : {
					UniLongObject lo = (UniLongObject)o;
					str+="\nData content:"+lo.getDataContent();
					break; }
				case FLOAT_TYPE : {
					UniFloatObject fo = (UniFloatObject)o;
					str+="\nData content:"+fo.getDataContent();
					break; }
				case STRING_TYPE : {
					UniStringObject so = (UniStringObject)o;
					str+="\nData content:"+so.getDataContent();
					break; }
				case VOID_TYPE : {
					UniVoidObject so = (UniVoidObject)o;
					str+="\nData content:"+so.getDataContent();
					break; }
				default: str+="-ERROR-Unknown data type!";
			}
		}

        return str;
        
    }

	
	
// private methods
	
	private void updateMessageObjectsCounter() {
		header.updateMessageObjectsCounter((byte)(uniObjects.size()));
	}

	
	private int getSizeOfObjectData(UniObject uniObj) {
		int dataSize = 0;
		
		switch(uniObj.getDataType()) {
			case BYTE_TYPE : { dataSize = BYTE_TYPE_SIZE; break; }
			case SHORT_TYPE :{ dataSize = SHORT_TYPE_SIZE; break; }
			case INT_TYPE : { dataSize = INT_TYPE_SIZE; break; }
			case LONG_TYPE :{ dataSize = LONG_TYPE_SIZE; break; }
			case FLOAT_TYPE :{ dataSize = FLOAT_TYPE_SIZE; break; }
			case VOID_TYPE : { dataSize = uniObj.getDataSize(); break; }
			case STRING_TYPE : { dataSize = uniObj.getDataSize(); break; }
		}
		return dataSize;
	}

} //Message
