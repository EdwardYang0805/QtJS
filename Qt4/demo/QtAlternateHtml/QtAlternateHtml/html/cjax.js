/*****************************************************
 *插件名称：CJAX
 *创建时间：2016年11月04日
 *创 建 人：EdwardYang
 *插件功能描述：
 *		提供Qt程序加载html界面显示时，html界面和Qt程序的交互功能
 *		html--->Qt  提供:
 *
 *********************************************************/
 
 //保存所有未完成的请求数据对象
var requestBobyList = [];
 
 //用于保存notify指令与回调函数的对应
var notifyCMDObjects= [];
 
 /***************************************
  *方法名称：CJax
  *参    数  requestBody  请求数据对象
  *功能描述：向Qt端发送request请求
  requestBody对象格式：
		｛
			url:"",			//请求指令
			type:"",		//请求方式（Get、POST、GO）
			timeout:3000,	//请求超时时间
			async：true,	//请求回应方式
			data:"",		//请求数据，之后在POST的时候使用。数据为Json格式的字符串
			success:,		//请求成功后回调函数，函数原型function(data){}
			error:,			//请求失败时被调用的函数,函数原型function(errNo){}
		｝
		
向Qt发送请求json格式
		｛
		
		｝
  **************************************/
var cjax = function(requestBody)
{
	 //将请求添加到请求列表中
	requestBobyList.push(requestBody);
	var paramObj = {};
	paramObj.url = requestBody.url;
	paramObj.type = requestBody.type;
	paramObj.timeout = requestBody.timeout;
	paramObj.data = requestBody.data;
	var str = JSON.stringify(paramObj);
	QtWebObj.dealRequest(str);
}
 
 /*********************************************************
  *方法名称：responseInterFace
  *参    数：responseBody  请求返回数据 json格式
  *功能说明：发送请求之后，Qt端返回的数据
		  {
			  "url":""，					//请求指令
			  "state":200,					//请求返回值
			  "datatype":"xml/json/text",	//返回数据类型
			  "data":""						//返回数据
		  }
  *********************************************************/
var responseInterFace = function(responseBody)
{
	var url = responseBody.url;
	for (var i = 0; i < requestBobyList.length; i++)
	{
		if(requestBobyList[i].url == url)
		{
			var callback;
			if (responseBody.state != 200)
				callback = requestBobyList[i].error;
			else
				callback = requestBobyList[i].success;
			callback.call(this, responseBody.datatype, responseBody.data);
			requestBobyList.splice(i, 1);
			break;
		}
	}
}
  
 /*********************************************************
  *方法名称：webInterFace
  *参    数：notifyObj Qt端推送过来的信息结构对象  	callback指令对应的毁掉函数
  *功能说明：Qt端主动推送消息通知接口
		｛
			"url":"指令"，					//消息指令
			"datatype"："xml/json/text",	//数据类型
			"data":""						//数据
		｝
  ***********************************************************/
var webInterFace = function(notifyObj)
{
	var url = notifyObj.url;
	for (var i = 0; i < notifyCMDObjects.length; i++)
	{
		if(notifyCMDObjects[i].notifyCmd == url)
		{
			var callback = notifyCMDObjects[i].func;
			callback.call(this, notifyObj.datatype, notifyObj.data);
			break;
		}
	}
}
  
  /*********************************************************
  *方法名称：insertNotifyCMDCallBack
  *参    数：cmd 指令  callback 回调函数
  *功能说明：保存所有QT端推送的指令和其对应的回调函数
  ***********************************************************/
var insertNotifyCMDCallBack = function(cmd,  callback)
{
	var cmdObject = {
		notifyCmd:cmd,
		func:callback
	}
	notifyCMDObjects.push(cmdObject);
}
  
