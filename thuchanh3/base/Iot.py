import paho.mqtt.client as mqtt
 
class Iot:
    _state = None
    _client = None
    _dict = {
        'straightforward': 0,
        'backwards': 1,
        'left': 2,
        'right':3,
    }
    def __init__(self, client):
        self._client = client
        
    def emit(self, name, event):
        if event != self._state:
            self._state = event
            self._client.publish("/dc", self._dict[event])
            print("emit /dc envent with value {} - {}".format(self._dict[event], name))
            

 
def on_message(topic, iot):
    data = topic.split("/")
    name = data[1]
    action = data[2]
    iot.emit(name, action)
         
        
client = mqtt.Client()
iot = Iot(client)
 
client.on_connect = lambda self, mosq, obj, rc: self.subscribe("/nhi/#")
client.on_message = lambda client, userdata, msg: on_message(msg.topic, iot)
 
client.connect("localhost", 1883, 60)
client.loop_forever()
