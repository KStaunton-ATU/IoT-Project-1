function doGet(e) { 
  Logger.log( JSON.stringify(e) );  // view parameters


  var result = 'Ok'; // assume success


  if (e.parameter == undefined) {
    result = 'No Parameters';
  }
  else {
    var id = '1x5b5rFN28Pq1N80P3Fvb0KA2p7eUCuH9fVPvxoWHF8I';//specific to the spreadsheet you are working on. this is id is found     //directly after "docs.google.com/spreadsheetURL/d" in the URL. 
   
    var sheet = SpreadsheetApp.openById(id).getActiveSheet();
    var newRow = sheet.getLastRow() + 1;
    var rowData = [];
    //var waktu = new Date();
    rowData[0] = new Date(); // Timestamp in column A
    
    for (var param in e.parameter) {
      Logger.log('In for loop, param='+param);
      var value = stripQuotes(e.parameter[param]);
      //Logger.log(param + ':' + e.parameter[param]);
      switch (param) {
        case 'devId': //Parameter
          rowData[1] = value; //Value in column B
          break;
        case 'LightValue':
          rowData[2] = value; //Value in column C
          break;
        default:
          result = "unsupported parameter";
      }
    }
    Logger.log(JSON.stringify(rowData));


    // Write new row below
    var newRange = sheet.getRange(newRow, 1, 1, rowData.length);
    newRange.setValues([rowData]);
  }


  // Return result of operation
  return ContentService.createTextOutput(result);
}


/**
* Remove leading and trailing single or double quotes
*/
function stripQuotes( value ) {
  return value.replace(/^["']|['"]$/g, "");
}
