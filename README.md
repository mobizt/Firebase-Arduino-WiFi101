# Firebase RTDB Arduino Client for ARM/AVR WIFI Dev Boards


Google's Firebase Realtime Database Arduino Library for ARM/AVR WIFI Development Boards 
based on WiFi101 library, v 1.0.4

This client library provides the most reliable operations for read, store, update Firebase RTDB through REST API.


The library supports Arduino WiFi Shield 101 and Arduino MKR1000 WIFI Boards. 

For Espressif MCUs, please try [ESP8266 Firebase Arduino library]( https://github.com/mobizt/Firebase-ESP8266) and [ESP32 Firebase Arduino library]( https://github.com/mobizt/Firebase-ESP32)



For Arduino MKR WiFi 1010, Arduino MKR VIDOR 4000 and Arduino UNO WiFi Rev.2, please try [Firebase Arduino Library for ARM/AVR WIFI Dev Boards based on WiFiNINA](https://github.com/mobizt/Firebase-Arduino-WiFiNINA)


Copyright (c) 2019 K. Suwatchai (Mobizt).





## Tested Devices


This following devices were tested and work well.

 * Arduino MKR1000 WIFI





## Features


* **Not required fingerprint** or **certificate data** to connect.

* **Read data** at the defined database path using get functions e.g. **getInt**, **getFloat**, **getBool**, **getString** and **getJSON**.

* **Store data** at the defined database path using set functions e.g. **setInt**, **setFloat**, **setBool**, **setString** and  **setJSON**.

* **Append data** to the defined database path using push functions e.g. **pushInt**, **pushFloat**, **pushBool**,  **pushString** and **pushJSON**.

* **Update data** at the defined database path using **updateNode** and **updateNodeSilent** functions.

* **Delete data** at the defined database path (include all child nodes) using **deleteNode** function.

* Using **Firebase Data object** that holds all data and instances.

* Supports **Data Filtering** using the orderBy, limitToFirst, limitToLast, startAt, endAt, and equalTo query parameters.

* Supports integer, float, boolean, string and JSON string data types.

  For JSON string data type, parsing as an object required external JSON parser library e.g. [**ArduinoJson**](https://github.com/bblanchon/ArduinoJson).

* Supports **automatic stream resuming** when the stream connection was timeout. 

* Supports **Firebase pause**.






## Dependencies


This library required [WiFi101 Library](https://github.com/arduino-libraries/WiFi101) to be installed which can be installed through **Boards Manager**


## Installing


### Using Library Manager


At Arduino IDE goto menu **Sketch** -> **Include Library** -> **Manage Libraries...**


In Library Manager Window, search **"firebase"** in the search form then select **"Firebase Arduino based on WiFi101"** and click **"Install"** button.



### Manual installing


Click at **Clone or download** dropdown at the top of repository, select **Download ZIP** and save file on your computer.


From Arduino IDE, goto menu **Sketch** -> **Include Library** -> **Add .ZIP Library...** and choose **Firebase-Arduino-WiFiNIN A-master.zip** that previously downloaded.


Go to menu **Files** -> **Examples** -> **Firebase-Arduino-WiFiNIN A-master** and choose one from examples.





## Usages


### Declaration and Initialization


#### The first thing to do to use this library.

```C++

//1. Include Firebase Arduino WiFi101 library (this library)

#include "Firebase_Arduino_WiFi101.h"


//2. Declare the Firebase Data object in global scope

FirebaseData firebaseData;

//3. Setup Firebase credential in setup()

Firebase.begin("yout_project_id.firebaseio.com", "your_Firebase_database_secret", "your_wifi_ssid", "your_wifi_password");

//4. Optional, set AP reconnection in setup()

Firebase.reconnectWiFi(true);

```

___





### Read, Store, Update, Delete, Backup and Restore Data


**To read the data, use `get<Data Type>` functions i.e. getInt, getFlot, getBool, getString and getJSON.**


The `get<Data Type>` function returned boolean value for success of operation. The success of operation determined from

payload that Firebase server returned back to client. The http status and matching between data type request and response were determined to set the success status.

To read the payload data, one of theses functions can be called i.e. intData, floatData, boolData, stringData and jsonData.

The data you read from returned payload will tell actual data type stored or existed in database, not the modification version data type e.g. string "1.5" stored in database, can be read only from stringData as it returned from server.

Any attempt to read integer from intData, float from floatData, and JSON from JsonData will return zero and empty string. 

This allow you to know what exactly type of data stored in database and how to manage it instead of cast all data as string (int -> string or float -> string) or as number. 

You can call getDataType to determine what type of data returned to be manipulated.


Here is the example usage to read integer value from defined database path "/test/int".


```C++

//Read integer value from database at "/test/int" 

  int val = 0;

  if (Firebase.getInt(firebaseData, "/test/int")) {

    //Success, then read the payload value

    //Make sure payload value returned from server is integer
    //This prevent you to get garbage data
    if (firebaseData.dataType() == "int")) {
      val = firebaseData.intData();
      Serial.println(val);
    }

  } else {
    //Failed, then print out the error detail
    Serial.println(firebaseData.errorReason());
  }

```





**To store the data, use `set<Data Type>` functions i.e. setInt, setFlot, setBool, setString and setJSON.**


The `set<Data Type>` function returned boolean value for success of operation. The success of operation determined from

payload that Firebase server returned back to client. The http status and matching between data type request and response.

Below is the example usage to store or set float value to database at "/test/float_data".


```C++


if (Firebase.setFloat(firebaseData, "/test/float_data", 123.456789)){

  //Success, then read the payload value return from server
  //This confirmed that your data was set to database as float number

  if (firebaseData.dataType() == "float")
    Serial.println(firebaseData.floatData());

} else {
  //Failed, then print out the error detail
    Serial.println(firebaseData.errorReason());
}

```





**To append new data to database, `push<Data Type>` should be called e.g. pushInt, pushFloat, pushBool, pushString and pushJSON.**


With push operation, server will return payload (key or name of newly appended node) to client.

Working with JSON data allow us to read or store multiple data at once because JSON data can store many key/value pairs, array of object and nested objects.

Function setJSON will set/replace value at defined database path with value in JSON data, and also create child nodes.  

While in function pushJSON, all key/value in JSON data  will be appended to the defined database path as new node.

Below is the example for appending new data (using JSON) to the path "/test/append.


```C++

//Append many data (multiple keys included nest data) to the database at "/test/append"

String jsonData = "{\"parent_001\":\"parent 001 text\", \"parent 002\":{\"child_of_002\":123.456}}";

if (Firebase.pushJSON(firebaseData, "/test/append", jsonData)) {

  //Success, then read the payload value

  //Database path to be appended
  Serial.println(firebaseData.dataPath()); //Should be "/test/append"

  //New created key/name
  Serial.println(firebaseData.pushName());

  //Absolute path of new appended data
    Serial.println(firebaseData.dataPath() + "/"+ firebaseData.pushName());


} else {
  //Failed, then print out the error detail
  Serial.println(firebaseData.errorReason());
}

```





**To update database at defined path and its child nodes, updateNode or updateNodeSilent should be called.**


JSON data is needed, and passed to these functions. The result from update operation, the database data at defined path will be partialy or completed update depending on the keys in JSON data. 

If any key provided in JSON data was not existed in database at the path, new key will be created.

Everytime you call updateNode, the payload that exactly the JSON data you sent will return back from server. 

Update database with large JSON will consume as much as double network data. Function updateNodeSilent can reduce the network data usage in this case.

Below is the example for database update at "/test" using JSON data.


```C++

//Append many data (multiple keys included nest data) to the database at "/test/update"

String updateData = "{\"data1\":\"value1\", \"data2\":{\"_data2\":\"_value2\"}}";

if (Firebase.updateNode(firebaseData, "/test/update", updateData)) {

  //Success, then try to read the payload value

  //Database path that updated
  Serial.println(firebaseData.dataPath());

  //Data type at updated database path
  Serial.println(firebaseData.dataType()); //Should be "json"

  //Print the JSON string payload that returned from server
  Serial.println(firebaseData.jsonData()); //Should mathes the value in updateData variable

  //Actual sent payload JSON data
  Serial.println(updateData);

} else {
  //Failed, then print out the error detail
  Serial.println(firebaseData.errorReason());
}

```





**To delete the data in database, just call deleteNode function.**


Below example will delete data and its child nodes at "/test/append"

```C++

Firebase.deleteNode(firebaseData, "/test/append");

```





**Data Filtering via query parameters**

The quey parameters that can set using the QueryFilter class.

These parameters are `orderBy`, `limitToFirst`, `limitToLast`, `startAt`, `endAt`, and `equalTo`.

To filter data, parameter `orderBy` should be assigned which you can assign "$key" for filtering all nodes under defined database path
using their keys, assign "$value" for filtering all nodes under defined database path using their values. 

Assign specific node key or name for filtering the specified child node under defined database path using their key, and assign "$priority" for filtering all nodes under defined database path using their "virtual child" named .priority.

And using the follower query properties to limit the queries.

`QueryFilter.limitToFirst` -  The total children (number) to filter from the first child.

`QueryFilter.limitToLast` -   The total last children (number) to filter. 

`QueryFilter.startAt` -       Starting value of range (number or string) of query upon orderBy param.

`QueryFilter.endAt` -         Ending value of range (number or string) of query upon orderBy param.

`QueryFilter.equalTo` -       Value (number or string) matches the orderBy param



Below example show how to using queries parameter in QueryFilter class to filter the data at database path "/test/data"

```C++

//Assume that children that have key "sensor" are under "/test/data"

//Instantiate the QueryFilter class
QueryFilter query;

//Clear query parameters.
query.clearQuery();

//Build query using specified child node key "sensor" under "/test/data"
query.orderBy("sensor");

//Query any child that its value is begin with 2 (number), assumed that its data type is float or integer
query.startAt(2);

//Query any child that its value is end with 8 (number), assumed that its data type is float or integer
query.endAt(8);

//Limit the maximum query result to return only the last 5 nodes
query.limitToLast(5);


if (Firebase.getJSON(firebaseData, "/test/data", query))
{
  //Success, then try to read the JSON payload value
  Serial.println(firebaseData.jsonData());
}
else
{
  //Failed to get JSON data at defined database path, print out the error reason
  Serial.println(firebaseData.errorReason());
}

 //Release memory used by query object
query.end();



```



___






### Data changes monitoring (Stream)


**The Firebase Realtime Database update or change event was sent to client using the HTTP stream connection. The connection was keep alive as long as no network communication interruption.**

The function to handle the update and change event monitoring are beginStream, readStream and streamAvailable.

To subscribe, monitor or follow the data change on defined database path, the function beginStream must be called first.

To handle the data that will be received when database at that path updates or changes, the function readStream and streamAvailable should be called in loop.

To determine the stream data is available, function streamAvailable should be called immediately after readStream. 

The function streamAvailable returned true when stream data was received in buffer. 

Then stream data can be accessed directly by calling intData, floatData, stringData, jsonData and blobData.


Here is the example use of stream to handle the changes or updates at "/test/data".

```C++


//In setup(), set the streaming path to "/test/data" and begin stream connection

if (!Firebase.beginStream(firebaseData, "/test/data"))
{
  //Could not begin stream connection, then print out the error detail
  Serial.println(firebaseData.errorReason());
}

//In loop()


 if (!Firebase.readStream(firebaseData))
  {
    //If read stream was failed, print the error detail.
    Serial.println(firebaseData.errorReason());
  }

  if (firebaseData.streamTimeout())
  {
     //If stream timeout, just notify
    Serial.println("Stream timeout, resume streaming...");
    Serial.println();
  }

  if (firebaseData.streamAvailable())
  {
   
   //Print out value
   //Stream data can be many types which can be determined from function dataType

    if (firebaseData.dataType() == "int")
      Serial.println(firebaseData.intData());
    else if (firebaseData.dataType() == "float")
      Serial.println(firebaseData.floatData());
    else if (firebaseData.dataType() == "boolean")
      Serial.println(firebaseData.boolData());
    else if (firebaseData.dataType() == "string")
      Serial.println(firebaseData.stringData());
    else if (firebaseData.dataType() == "json")
      Serial.println(firebaseData.jsonData());
    
  }



```



See [full examples](https://github.com/mobizt/Firebase-Arduino-WiFi101/tree/master/examples) for all features usages.





## All Supported Functions


**These are all functions available from the library and the descriptions.**


### Global functions


**Store Firebase's authentication credentials.**

param **`host`** - Your Firebase database project host without http:// or https:// protocol e.g. Your_ProjectID.firebaseio.com.

param **`auth`** - Your database secret.

param **`wifiSSID`** - Your WiFi AP SSID.

param **`wifiPSW`** - Your WiFi AP Password.

```C++
void begin(const String &host, const String &auth, const String &wifiSSID, const String &wifiPSW);
```





**Reconnect WiFi if lost connection.**

param **`reconnect`** - The boolean to set/unset WiFi AP reconnection.

```C++
void reconnectWiFi(bool reconnect);
```





**Append new integer value to the defined database path.**

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Target database path which integer value will be appended.

param **`intValue`** - The appended value.

return **`Boolean`** type status indicates the success of operation.

The new appended node's key will be stored in Firebase Data object, 
which its value can be accessed via function [FirebaseData object].pushName().

```C++
bool pushInt(FirebaseData &dataObj, const String &path, int intValue);
```





**Append new float value to the defined database path.**

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Target database path which float value will be appended.

param **`floatValue`** - The appended value.

return **`Boolean`** type status indicates the success of operation.

The new appended node's key will be stored in Firebase Data object, 
which its value can be accessed via function [FirebaseData object].pushName().

```C++
bool pushFloat(FirebaseData &dataObj, const String &path, float floatValue);
```





**Append new Boolean value to the defined database path.**

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Target database path which Boolean value will be appended.

param **`boolValue`** - The appended value.

return **`Boolean`** type status indicates the success of operation.

The new appended node's key will be stored in Firebase Data object, 
which its value can be accessed via function [FirebaseData object].pushName().

```C++
bool pushBool(FirebaseData &dataObj, const String &path, bool boolValue);
```




**Append new string (text) to the defined database path.**

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Target database path which string will be appended.

param **`stringValue`** - The appended value.

return **`Boolean`** type status indicates the success of operation.

The new appended node's key stored in Firebase Data object, 
which can be accessed via function [FirebaseData object].pushName().

```C++
bool pushString(FirebaseData &dataObj, const String &path, const String &stringValue);
```





**Append new child nodes's key and value (using JSON data) to the defined database path.**

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Target database path which key and value in JSON data will be appended.

param **`jsonString`** - The appended JSON string (should be valid JSON data).

return **`Boolean`** type status indicates the success of operation.

The new appended node's key will be stored in Firebase Data object, 
which its value can be accessed via function [FirebaseData object].pushName().

```C++
bool pushJSON(FirebaseData &dataObj, const String &path, const String &jsonString);
```






**Append new Firebase server's timestamp to the defined database path.**

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Target database path which timestamp will be appended.

return - **`Boolean`** type status indicates the success of operation.
    
The new appended node's key will be stored in Firebase Data object, 
which its value can be accessed via function [FirebaseData object].pushName().

```C++
bool pushTimestamp(FirebaseData &dataObj, const String &path);
```





**Set integer data at the defined database path.**

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Target database path which integer data will be set.

param **`intValue`** - Integer value to set.

return **`Boolean`** type status indicates the success of operation.

Call [FirebaseData object].dataType to determine what type of data that successfully
stores in database. 
    
Call [FirebaseData object].intData will return the integer value of
payload returned from server.

```C++
bool setInt(FirebaseData &dataObj, const String &path, int intValue);
```





**Set float data at the defined database path.**

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Target database path which float data will be set.

param **`floatValue`** - Float value to set.

return **`Boolean`** type status indicates the success of operation.

Call [FirebaseData object].dataType to determine what type of data that successfully
stores in database. 

Call [FirebaseData object].floatData will return the float value of
payload returned from server.

```C++
bool setFloat(FirebaseData &dataObj, const String &path, float floatValue);
```





**Set Boolean data at the defined database path.**

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Target database path which float data will be set.

param **`boolValue`** - Boolean value to set.

return **`Boolean`** type status indicates the success of operation.

Call [FirebaseData object].dataType to determine what type of data that successfully
stores in database. 

Call [FirebaseData object].boolData will return the Boolean value of
payload returned from server.

```C++
bool setBool(FirebaseData &dataObj, const String &path, bool boolValue);
```





**Set string (text) at the defined database path.**

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Target database path which string data will be set.

param **`stringValue`** - String or text to set.

return **`Boolean`** type status indicates the success of operation.

Call [FirebaseData object].dataType to determine what type of data that successfully
stores in database. 

Call [FirebaseData object].stringData will return the string value of
payload returned from server.

```C++
bool setString(FirebaseData &dataObj, const String &path, const String &stringValue);
```





**Set child nodes's key and value (using JSON data) to the defined database path.**

This will replace any child nodes inside the defined path with node' s key
and value defined in JSON data.

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Target database path which key and value in JSON data will be replaced or set.

param **`jsonString`** - The JSON string to set (should be valid JSON data).

return **`Boolean`** type status indicates the success of operation.

Call [FirebaseData object].dataType to determine what type of data that successfully
stores in database. 
 
Call [FirebaseData object].jsonData will return the JSON string value of
payload returned from server.

```C++
bool setJSON(FirebaseData &dataObj, const String &path, const String &jsonString);
```






**Set Firebase server's timestamp to the defined database path**

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Target database path which timestamp will be set.

return - **`Boolean`** type status indicates the success of operation.
    
Call [FirebaseData object].intData will return the integer value of timestamp returned from server.

```C++
bool setTimestamp(FirebaseData &dataObj, const String &path);
```







**Update child nodes's key or exising key's value (using JSON data) under the defined database path.**

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Target database path which key and value in JSON data will be update.

param **`jsonString`** - The JSON string use for update.

return **`Boolean`** type status indicates the success of operation.

Call [FirebaseData object].dataType to determine what type of data that successfully
stores in database. 
 
Call [FirebaseData object].jsonData will return the json string value of
payload returned from server.

To reduce the network data usage, use updateNodeSilent instead.

```C++
bool updateNode(FirebaseData &dataObj, const String &path, const String &jsonString);
```





**Update child nodes's key or exising key's value (using JSON data) under the defined database path.**

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Target database path which key and value in JSON data will be update.

param **`jsonString`** - The JSON string use for update.

return **`Boolean`** type status indicates the success of operation.

Owing to the objective of this function to reduce the netwok data usage, 
no payload will be returned from server.

```C++
bool updateNodeSilent(FirebaseData &dataObj, const String &path, const String &jsonString);
```





**Read the integer value at the defined database path.**

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Database path which the integer value is being read.

return **`Boolean`** type status indicates the success of operation.

Call [FirebaseData object].dataType to determine what type of data that successfully
stores in database. 

Call [FirebaseData object].intData will return the integer value of
payload returned from server.

If the payload returned from server is not integer or float type, 
the function [FirebaseData object].intData will return zero (0).

If the payload returned from server is float type, 
the function [FirebaseData object].intData will return rounded integer value.

```C++
bool getInt(FirebaseData &dataObj, const String &path);
```





**Read the float value at the defined database path.**

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Database path which the float value is being read.

return **`Boolean`** type status indicates the success of operation.

Call [FirebaseData object].dataType to determine what type of data that successfully
stores in database. 

Call [FirebaseData object].floatData will return the float value of
payload returned from server.

If the payload returned from server is not integer or float type, 
the function [FirebaseData object].intData will return zero (0).

```C++
bool getFloat(FirebaseData &dataObj, const String &path);
```





**Read the Boolean value at the defined database path.**

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Database path which the Boolean value is being read.

return **`Boolean`** type status indicates the success of operation.

Call [FirebaseData object].dataType to determine what type of data that successfully
stores in database. 

Call [FirebaseData object].boolData will return the Boolean value of
payload returned from server.

If the payload returned from server is not boolean type, 
the function [FirebaseData object].boolData will return false.

```C++
bool getBool(FirebaseData &dataObj, const String &path);
```






**Read the string or text at the defined database path.**

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Database path which the string value is being read.

return **`Boolean`** type status indicates the success of operation.

Call [FirebaseData object].dataType to determine what type of data that successfully
stores in database. 

Call [FirebaseData object].stringData will return the string value of
payload returned from server.

If the payload returned from server is not string type, 
the function [FirebaseData object].stringData will return empty string (String object).

```C++
bool getString(FirebaseData &dataObj, const String &path);
```





**Read the JSON string at the defined database path.**

The returned payload JSON string represents the child nodes and their value.

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Database path which the string value is being read.

return **`Boolean`** type status indicates the success of operation.

Call [FirebaseData object].dataType to determine what type of data that successfully
stores in database. 

Call [FirebaseData object].jsonData will return the JSON string value of
payload returned from server.

If the payload returned from server is not json type, 
the function [FirebaseData object].jsonData will return empty string (String object).

```C++
bool getJSON(FirebaseData &dataObj, const String &path);
```





**Read the JSON string with data filtering at the defined database path.**

The returned payload JSON string represents the child nodes and their value.

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Database path which the string value is being read.

param **`query`** - QueryFilter class to set query parameters to filter data.

return **`Boolean`** type status indicates the success of operation.

Available query parameters for filtering the data are the following.

**`QueryFilter.orderBy`** - Required parameter to specify which data used for data filtering included child key, key and value.

Use "$key" for filtering data by keys of all nodes at the defined database path.

Use "$value" for filtering data by value of all nodes at the defined database path.

Use "$priority" for filtering data by "virtual child" named .priority of all nodes.

Use any child key to filter by that key.


**`QueryFilter.limitToFirst`** -  The total children (number) to filter from the first child.

**`QueryFilter.limitToLast`** -   The total last children (number) to filter. 

**`QueryFilter.startAt`** -       Starting value of range (number or string) of query upon orderBy param.

**`QueryFilter.endAt`** -         Ending value of range (number or string) of query upon orderBy param.

**`QueryFilter.equalTo`** -       Value (number or string) matches the orderBy param


Call [FirebaseData object].dataType to determine what type of data that successfully
stores in database. 

Call [FirebaseData object].jsonData will return the JSON string value of
payload returned from server.

If the payload returned from server is not json type, 
the function [FirebaseData object].jsonData will return empty string (String object).

[FirebaseData object].jsonData will return null when the filtered data is empty.

```C++
bool getJSON(FirebaseData &dataObj, const String &path, QueryFilter &quer);
```






**Delete all child nodes at the defined database path.**

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Database path to be deleted.

return **`Boolean`** type status indicates the success of operation.*

```C++
bool deleteNode(FirebaseData &dataObj, const String &path);
```





**Start monitoring the value changes at the defined path and its children.**

param **`dataObj`** - Firebase Data Object to hold data and instances.

param **`path`** - Database path being monitor.

return **`Boolean`** type status indicates the success of operation.*

```C++
bool beginStream(FirebaseData &dataObj, const String &path);
```





**Read the stream event data at defined database path.**

Once beginStream was called e.g. in setup(), the readStream function
should call inside the loop function.

param **`dataObj`** - Firebase Data Object to hold data and instances.

return **`Boolean`** type status indicates the success of operation.

Using the same Firebase Data object for stream read/monitoring associated 
with read, store, update, delete will break or quit the current stream connection. 
    
The stream will be resumed or reconnected automatically when calling readStream.

```C++
bool readStream(FirebaseData &dataObj);
```





**End the stream connection at defined path.**

Can be restart again by calling beginStream.

param **`dataObj`** - Firebase Data Object to hold data and instances.

return **`Boolean`** type status indicates the success of operation.
 
```C++
bool endStream(FirebaseData &dataObj);
```






### Firebase Data object functions


**Get WiFi client instance.**


return **`WiFi client instance`**.

```C++
WiFiSSLClient getWiFiClient();
```





**Pause/Unpause WiFiClient from all Firebase operations.**

param **`pause`** True for pause and False for unpause.

return **`Boolean`** type status indicates the success of operation.

```C++
bool pauseFirebase(bool pause);
```





**Determine the data type of payload returned from server.**

return *The one of these data type e.g. integer, float, string, json and blob.*

```C++
String dataType();
```






**Determine the event type of stream.**

return The one of these event type String e.g. put, patch, cancel, and auth_revoked.

The event type "put" indicated that data at event path relative to stream path was completely changed. Event path can be determined from dataPath().

The event type "patch" indicated that data at event path relative to stream path was updated. Event path can be determined from dataPath().

The event type "cancel" indeicated something wrong and cancel by server.

The event type "auth_revoked" indicated the provided Firebase Authentication Data (Database secret) is no longer valid.

```C++
String eventType();
```






**Determine the current stream path.**

return *The database streaming path.*

```C++
String streamPath();
```





**Determine the current data path.**

return *The database path which belong to server' s returned payload.*

The database path returned from this function in case of stream, also changed up on the child or parent's stream
value changes.

```C++
String dataPath();
```





**Determine the error reason String from process.**

return *The error description string (String object).*

```C++
String errorReason();
```





**Return the ineger data of server returned payload.**

return *Integer value.*

```C++
int intData();
```





**Return the float data of server returned payload.**

return *Float value.*

```C++
float floatData();
```






**Return the Boolean data of server returned payload.**

return *Boolean value.*

```C++
float boolData();
```






**Return the String data of server returned payload.**

return *String (String object).*

```C++
String stringData();
```





**Return the JSON String data of server returned payload.**

return *String (String object).*

```C++
String jsonData();
```






**Return the new appended node's name or key of server returned payload when calling pushXXX function.**

return **`String`** (String object).

```C++
String pushName();
```





**Determine the stream connection status.**

return **`Boolean`** type status indicates whether the Firebase Data object is working with stream or not.

```C++
bool isStream();
```





**Determine the server connection status.**

return **`Boolean`** type status indicates whether the Firebase Data object is connected to server or not.

```C++
bool httpConnected();
```





**Determine the timeout event of server's stream (30 sec is default).**

Nothing to do when stream connection timeout, the stream connection will be automatic resumed.

return **`Boolean`** type status indicates whether the stream was timeout or not.

```C++
bool streamTimeout();
```





**Determine the availability of data or paylaod returned from server.**

return **`Boolean`** type status indicates whether the server return back the new payload or not.

```C++
bool dataAvailable();
```





**Determine the availability of stream event-data paylaod returned from server.**

return **`Boolean`** type status indicates whether the server return back the stream event-data 
payload or not.

```C++
bool streamAvailable();
```





**Determine the matching between data type that intend to get from/store to database and the server's return payload data type.**

return **`Boolean`** type status indicates whether the type of data being get from/store to database 
and server's returned payload are matched or not.

```C++
bool mismatchDataType();
```





**Determine the http status code return from server.**

return **`Integer`** number of HTTP status.

```C++
int httpCode();
```





**Check overflow of the returned payload data buffer.**

return **`Boolean`** of the overflow status.


Default buffer size is 400 bytes, assigned via FIREBASE_RESPONSE_SIZE macro in FirebaseESP8266.h


```C++
bool bufferOverflow();
```




**Release memory used by Firebase Data object.**

```C++
void end();
```





**Return the server's payload data.**

return *Payload string* (String object).

```C++
String payload();
```





## License

The MIT License (MIT)

Copyright (c) 2019 K. Suwatchai (Mobizt)


Permission is hereby granted, free of charge, to any person returning a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


