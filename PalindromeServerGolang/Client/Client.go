package main

import (
	"net"
	"github.com/s-gheldd/PalindromeServer"
	"log"
	"os"
	"fmt"
)

func main() {
	if len(os.Args) != 2 {
		log.Fatal("adress must be provided")
	}
	conn, err := net.Dial("tcp", os.Args[1] + PalindromeServer.PALINDROME_SERVER_PORT_NUMBER)
	if err != nil {
		log.Fatal(err)
	}

	for {
		message := ""
		fmt.Scanf("%s", &message)
		fmt.Fprintf(conn, "%s\n", message)
		if (message == "quit" || message == "exit") {
			return
		}
		fmt.Fscanf(conn, "%s", &message)
		fmt.Println(message)
	}
}
