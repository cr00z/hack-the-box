import static com.fasterxml.jackson.databind.*;
import java.nio.file;

public class MyValue {
	public value = "111";
}

public class Test {
	public static void main(String[] args) {
		// Serialize then deserialize MyValue object instance from a file.
		static ObjectMapper mapper = new ObjectMapper(); // can use static singleton
		MyValue value = new MyValue();
		File f = new File("serializationdata.json");
		// Serialize MyValue instance to file
		mapper.writeValue(f, value);
		// Deserialize MyValue instance from file
		MyValue value = mapper.readValue(f, MyValue.class);
	}
}
