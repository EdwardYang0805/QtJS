window.onload = function(){
	
	var cmd = "clear";
	var callback = clear;
	insertNotifyCMDCallBack(cmd, callback);
}


var showData = function (dataType, data)
{
	if (dataType == "Text")
		$("#input1").val(data);
}

var dealError = function(){
	
	alert("error");
}

var clear = function(datatype, datatype)
{
	$("#input1").val("");
}



var setTextValue = function()
{
	var requestBody = {
		url: "setTextValue",					//请求指令
		type: "Get",							//请求方式（Get、POST、GO）
		timeout: 3000,							//请求超时时间
		async: true,							//请求回应方式
		data: "",								//请求数据，之后在POST的时候使用。数据为Json格式的字符串
		success: showData,						//请求成功后回调函数，函数原型function(data){}
		error: dealError,								//请求失败时被调用的函数,函数原型function(errNo){}
	}
	cjax(requestBody);
}

var turntoPage = function()
{
	window.location.href  = "test.html";
}

  /*********************************************************
  *方法名称：msgDataNotify
  *参    数：Jsondata  	json数据
  *功能说明：Qt端主动消息通知接口
  ***********************************************************/
var msgDataNotify = function(Jsondata)
{
  
}
  
  /*********************************************************
  *方法名称：msgAlarmNotify
  *参    数：Jsondata  	json数据
  *功能说明：Qt端主动消息通知接口
  ***********************************************************/
var msgAlarmNotify = function(Jsondata)
{
  
}
  
   /*********************************************************
  *方法名称：msgDevStateNotify
  *参    数：Jsondata  	json数据
  *功能说明：Qt端主动消息通知接口
  ***********************************************************/
var msgDevStateNotify = function(Jsondata)
{
  
}