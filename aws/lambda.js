var AWS = require('aws-sdk')
var iotdata = new AWS.IotData({ endpoint: '.iot.regionamazonaws.com' })

exports.handler = async(event) => {
    console.log("Event => " + JSON.stringify(event))
    var params = {
        topic: "inTopic",
        payload: JSON.stringify(event["queryStringParameters"]),
        qos: 0
    }

    return iotdata.publish(params, function(err, data) {
        if (err) {
            console.log("ERROR => " + JSON.stringify(err))
        }
        else {
            console.log("Success")
        }
    }).promise()
}
