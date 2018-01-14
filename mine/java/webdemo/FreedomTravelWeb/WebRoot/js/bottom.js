	$(document).ready(
		function()
		{
			$('#dock2').Fisheye(
				{
					maxWidth: 70,
					items: 'a',
					itemsText: 'span',
					container: '.dock-container2',
					itemWidth: 70,
					proximity: 80,
					alignment : 'left',
					valign: 'bottom',
					halign : 'center'
				}
			)
		}
	);