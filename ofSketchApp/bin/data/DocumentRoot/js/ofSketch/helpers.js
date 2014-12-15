function getPrettyFileName(fileName) {
  var periodIndex = fileName.lastIndexOf('.');
  return (periodIndex == -1) ? fileName : fileName.substring(0, periodIndex);
}

/// \brief Load a file
function loadFile(filename, callback) {
  $.ajax({
    url: filename,
    success: function(data) {
      callback(data);
    },
    error: function(err) {
      throw err;
    }
  });
}

/// \brief Get the value for a URL parameter.
/// \param name The URL parameter to get.
/// \returns the value for the key name.
function getURLParameter(name) {
  name = name.replace(/[\[]/, "\\[").replace(/[\]]/, "\\]");
  var regex = new RegExp("[\\?&]" + name + "=([^&#]*)"),
    results = regex.exec(location.search);
  return results == null ? undefined : decodeURIComponent(results[1].replace(
    /\+/g, " "));
}

/// \brief Get a unique id (UUID).
/// \returns a unique id (UUID).
function generateUUID() {
  return s4() + s4() + '-' + s4() + '-' + s4() + '-' +
    s4() + '-' + s4() + s4() + s4();

  function s4() {
    return Math.floor((1 + Math.random()) * 0x10000)
      .toString(16)
      .substring(1);
  }
};
