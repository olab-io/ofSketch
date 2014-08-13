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

	this.init = function(projectName){

		$('#fileupload').fileupload({
		    url: _uploadUrl,
		    dataType: 'json',
		    method: 'POST',
		    formData: {
		        projectName: projectName
		    },
		    autoUpload: false,
		    acceptFileTypes: /(\.|\/)(gif|jpe?g|png|mov|mp3)$/i,
		    maxFileSize: 5000000*1024, // 5 GB
		    // Enable image resizing, except for Android and Opera,
		    // which actually support image resizing, but fail to
		    // send Blob objects via XHR requests:
		    disableImageResize: /Android(?!.*Chrome)|Opera/
		        .test(window.navigator.userAgent),
		    previewMaxWidth: 100,
		    previewMaxHeight: 100,
		    previewCrop: true
		    }).on('fileuploadadd', function (e, data) {
		        data.context = $('<div/>').appendTo('#files');
		        $.each(data.files, function (index, file) {

		            var node = $('<p/>')
		                    .append($('<span/>').text(file.name));
		            if (!index) {
		                node
		                    .append('<br>')
		                    .append(_uploadButton.clone(true).data(data));
		            }
		            node.appendTo(data.context);
		                                        
		        });
		    }).on('fileuploadprocessalways', function (e, data) {
		        var index = data.index,
		            file = data.files[index],
		            node = $(data.context.children()[index]);
		        if (file.preview) {
		            node
		                .prepend('<br>')
		                .prepend(file.preview);
		        }
		        if (file.error) {
		            node
		                .append('<br>')
		                .append(file.error);
		        }
		        if (index + 1 === data.files.length) {
		            data.context.find('button')
		                .text('Upload')
		                .prop('disabled', !!data.files.error);
		        }
		    }).on('fileuploadprogressall', function (e, data) {
		        var progress = parseInt(data.loaded / data.total * 100, 10);
		        $('#progress .progress-bar').css(
		            'width',
		            progress + '%'
		        );
		    }).on('fileuploaddone', function (e, data) {
		    	console.log('File upload done');
		        $.each(data.result.files, function (index, file) {
		            var link = $('<a>')
		                .attr('target', '_blank')
		                .prop('href', file.url);
		            $(data.context.children()[index])
		                .wrap(link);
		        });
		    }).on('fileuploadchunkdone', function(){
		    	console.log('chunk done');
		    }).on('fileuploadfail', function (e, data) {
		       
		        $.each(data.files, function (index, file) {
		            var error = $('<span/>').text(file.error);
		            $(data.context.children()[index])
		                .append('<br>')
		                .append(error);
		        });
		    }).prop('disabled', !$.support.fileInput)
		        .parent().addClass($.support.fileInput ? undefined : 'disabled');

		    $('#fileupload').bind('fileuploaddone', function (e, data) {
		    	console.log('file uploaded');
		    });
	} 
}
