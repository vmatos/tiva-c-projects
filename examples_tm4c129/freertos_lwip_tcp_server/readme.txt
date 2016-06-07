FreeRTOS Example

This application utilizes FreeRTOS to perform a variety of tasks in a
concurrent fashion.  The following tasks are created:

* An lwIP task, listens to new connections on 8080, and echoes what receives. Then closes the connection.
