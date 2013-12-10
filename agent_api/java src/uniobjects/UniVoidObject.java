package uniobjects;
/**
 * @author Maciej Strozyk
 *
 */

public class UniVoidObject extends UniObject {

	private Object dataContent;
	
	/**
	 * Constructor
	 * 
	 * @param objectType
	 * @param dataContent
	 */
	public UniVoidObject(short objectType, Object dataContent) {
		
		this.objectType = objectType;
		dataType = generateDataType(objectType); 
		// TODO obliczyc rozmiar danej
		//dataSize = (byte)dataContent.
		this.dataContent = dataContent;
		
	}

	/**
	 * @return Returns the dataContent.
	 */
	public Object getDataContent() {
		return dataContent;
	}

	/**
	 * @param dataContent The dataContent to set.
	 */
	public void setDataContent(Object dataContent) {
		this.dataContent = dataContent;
	}

} // UniVoidObject
