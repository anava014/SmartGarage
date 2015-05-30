$(document).ready(function() {
	$('a').click(function(e) {
		e.preventDefault();
		$.get("/?trigger=1");
		$("#loadingImg").show();
		$("#loadingP").show();
		$.ajax({url:"#",success:function(){
			$("#loadingImg").hide();
			$("#loadingP").hide();
			
		}});

	});
});







