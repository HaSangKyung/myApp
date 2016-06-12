var express = require('express')
  , routes = require('./routes')
  , user = require('./routes/user')
  , http = require('http')
  , fs = require('fs')
  , mysql = require('mysql')
  , path = require('path');

var app = express();

var coninfo={host:'localhost', user:'root', password:'1234'};
var dbCon = mysql.createConnection( coninfo );
dbCon.connect();
dbCon.query('use test');

// all environments
app.set('port', process.env.PORT || 3000);
app.set('views', __dirname + '/views');
app.set('view engine', 'ejs');
app.use(express.favicon());
app.use(express.logger('dev'));
app.use(express.bodyParser());
app.use(express.methodOverride());
app.use(app.router);
app.use(express.static(path.join(__dirname, 'public')));

// development only
if ('development' == app.get('env')) {
  app.use(express.errorHandler());
}

app.get('/', routes.index);
app.get('/users', user.list);

http.createServer(app).listen(app.get('port'), function(){
  console.log('Express server listening on port ' + app.get('port'));
});



/* 자동차 목록 화면 호출 */
app.get('/carList', function(request, response) {
	var sql = "select * from car";
	dbCon.query(sql, function(err, result, fields) {
		if(err){
			response.send("쿼리오류..");
		}else{
			//console.log("결과:", result);
			response.render('carList',{carList:result, title: '자동차 목록'} );
		}
	});
});



/* 자동차 등록 화면 호출 */
app.get('/addCar', function(request, response) {
	response.render("addCar", { title: '자동차 정보 등록' } );
});



/* 자동차 등록 처리 */
app.post('/addCarInfo', function(request, response) {
	
	var car_name = request.body.CAR_NAME;
	var car_price = request.body.CAR_PRICE;
	var car_company = request.body.CAR_COMPANY;
	var car_year = request.body.CAR_YEAR;
	var car_id = Math.floor(Math.random()*99999999) + 10000000; // 키값 생성
	
	var sql = "insert into car(CAR_ID, CAR_NAME, CAR_COMPANY, CAR_PRICE, CAR_YEAR) values(?,?,?,?,?)";
	dbCon.query(sql,[car_id, car_name, car_company, car_price, car_year], function(err, result) {
		if(err){
			response.send("false");
		}else{
			response.send("true");
		}
	});
});



/* 자동차 수정 화면 호출 */
app.get('/updateCar', function(request, response) {
	var url = require('url');
	var params = url.parse(request.url, true);
	var carid = params.query.carid;
	
	console.log("request.url:", request.url);
	console.log("carid:", carid);
	
	var sql = "select * from car where CAR_ID = " + carid;
	dbCon.query(sql, function(err, result, fields) {
		if(err){
			response.send("쿼리오류..");
		}else{
			console.log("결과:", result);
			response.render('updateCar',{
				carInfo: result, 
				title: '자동차 정보 수정', 
				car_id: carid
			} );
		}
	});
});



/* 자동차 수정 처리 */
app.post('/updateCarInfo', function(request, response) {
	
	var car_id = request.body.CAR_ID;
	var car_name = request.body.CAR_NAME;
	var car_price = request.body.CAR_PRICE;
	var car_company = request.body.CAR_COMPANY;
	var car_year = request.body.CAR_YEAR;
	
	var sql = "update car set CAR_NAME = ?, CAR_COMPANY = ?, CAR_PRICE = ?, CAR_YEAR = ? where CAR_ID = ?";
	dbCon.query(sql,[car_name, car_company, car_price, car_year, car_id], function(err, result) {
		if(err){
			response.send("false");
		}else{
			response.send("true");
		}
	});
});



/* 자동차 삭제 처리 */
app.post('/delCarInfo', function(request, response) {
	
	var car_id = request.body.CAR_ID;
	console.log("car_id:", car_id);
	
	var sql = "delete from car where CAR_ID = ?";
	dbCon.query(sql,[car_id], function(err, result) {
		if(err){
			response.send("false");
		}else{
			response.send("true");
		}
	});
});
