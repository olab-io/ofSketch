// =============================================================================
//
// Copyright (c) 2014 Brannon Dorsey <http://brannondorsey.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================

function FileUploader() 
{
	var _uploadUrl = '/post';
	var _uploadButton = $('<button/>')
        .addClass('btn btn-primary')
        .prop('disabled', true)
        .text('Processing...')
        .on('click', function () {
            var $this = $(this),
                data = $this.data();
            $this
                .off('click')
                .text('Abort')
                .on('click', function () {
                    $this.remove();
                    data.abort();
                });
            data.submit().always(function () {
                $this.remove();
            });
        });

	this.init = function(projectName)
	{

		$('#fileupload').fileupload({
	        url: _uploadUrl,
	        dataType: 'json',
	        formData: {
		        projectName: projectName
		    },
	        progressall: function (e, data) {
	            var progress = parseInt(data.loaded / data.total * 100, 10);
	            $('#progress .progress-bar').css(
	                'width',
	                progress + '%'
	            );

	            if (progress == 100) {
	            	setTimeout(function(){
	            		$('#progress .progress-bar').css(
			                'width',
			                0 + '%'
			            );
	            	}, 1500);
	            }
	        }
	    }).on('fileuploaddone', function(e, data){
	    	$.each(data.result.files, function (index, file) {
	            var p = $('<p/>');
	            p.text(file.name).appendTo('#files');
	            p.append('<span style="float: right; color: #5cb85c" class="glyphicon glyphicon-ok"></span>');
	        });
	    }).on('fileuploadfail', function(e, data){
	    	// NOTE: this callback is not currently working
	    }).prop('disabled', !$.support.fileInput)
	        .parent().addClass($.support.fileInput ? undefined : 'disabled');
	}

	this.reset = function()
	{
		$('#files').empty();
		$('#progress .progress-bar').css('width', 0 + '%');
	}
}
