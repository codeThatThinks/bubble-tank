package main

import (
        "net/http"
        "fmt"
        "github.com/gorilla/websocket"
        "os"
        "os/exec"
)

var upgrader = websocket.Upgrader{}

func handleWebsocket(w http.ResponseWriter, r *http.Request) {
	c, err := upgrader.Upgrade(w, r, nil)

	if err != nil {
		return
	}

	defer c.Close()

	for {
		_, message, err := c.ReadMessage()
		if err != nil {
			break
		}

		fmt.Printf("%s\n", message)
		port, _ := os.OpenFile("/dev/ttyAMA0", os.O_APPEND|os.O_WRONLY, 0600);
		fmt.Fprintf(port, "%s\n", message)
		port.Close()
	}
}

func main() {
		fmt.Printf("Opening /dev/ttyAMA0 at 9600 baud\n")
		cmd := exec.Command("stty", "-F", "/dev/ttyAMA0", "speed", "9600", "cs8", "-cstopb", "-parenb")
		cmd.Start()

		fmt.Printf("Listening on 10.0.1.1:80\n")
        http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
        	http.ServeFile(w, r, "controller.html")
        })

        http.HandleFunc("/camera.jpg", func(w http.ResponseWriter, r *http.Request) {
        	cmd := exec.Command("fswebcam", "-r", "1280x720", "--no-banner", "--rotate", "180", "camera.jpg")
			cmd.Start()
			cmd.Wait()
        	http.ServeFile(w, r, "camera.jpg")
        })

        http.HandleFunc("/jquery.min.js", func(w http.ResponseWriter, r *http.Request) {
        	http.ServeFile(w, r, "jquery.min.js")
        })

        http.HandleFunc("/websocket", handleWebsocket);

        http.ListenAndServe(":80", nil)
}
