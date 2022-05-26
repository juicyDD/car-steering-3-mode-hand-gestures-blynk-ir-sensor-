

    
    function getAjax() {
      $.ajax({
          url: 'refresh-card/',
          success: function(data) {
              $('#mycard').html(data);
          }
      });

  }



    getAjax(); // This will run on page load
    setInterval(function(){
    getAjax() // this will run after every 5 seconds
        }, 1000);
    

      

