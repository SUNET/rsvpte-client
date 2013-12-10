package uniobjects;
/**
 * @author Maciej Strozyk
 *
 */

public class UniStringObject extends UniObject {

	private String dataContent;
	
	/**
	 * Constructor
	 * 
	 * @param objectType
	 * @param dataContent
	 */
	public UniStringObject(short objectType, String dataContent) {
		
		this.objectType = objectType;
		dataType = generateDataType(objectType); 
		dataSize = (byte)dataContent.length();
		this.dataContent = dataContent;
		
	}

	/**
	 * @return Returns the dataContent.
	 */
	public String getDataContent() {
		return dataContent;
	}

	/**
	 * @param dataContent The dataContent to set.
	 */
	public void setDataContent(String dataContent) {
		this.dataContent = dataContent;
	}

} // UniStringObject
