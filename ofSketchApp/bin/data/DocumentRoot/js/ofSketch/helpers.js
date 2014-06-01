function getPrettyFileName(fileName)
{
	var periodIndex = fileName.lastIndexOf('.');
	return (periodIndex == -1) ? fileName : fileName.substring(0, periodIndex);
}

function loadFile(filename, callback)
{
	$.ajax({
		url: filename,
		success: function(data){
			callback(data);
		},
		error: function(err){
			throw err;
		}
	});
}

function getURLParameter(name) {
    name = name.replace(/[\[]/, "\\[").replace(/[\]]/, "\\]");
    var regex = new RegExp("[\\?&]" + name + "=([^&#]*)"),
        results = regex.exec(location.search);
    return results == null ? undefined : decodeURIComponent(results[1].replace(/\+/g, " "));
}