package main

import (
	"net"
	"github.com/s-gheldd/PalindromeServer"
	"log"
	"fmt"
	"time"
)

func main() {
	exit := make(chan struct{})
	addr, err := net.ResolveTCPAddr("tcp", PalindromeServer.PALINDROME_SERVER_PORT_NUMBER)
	if err != nil {
		log.Fatal(err)
	}
	listener, err := net.ListenTCP("tcp", addr)
	if err != nil {
		log.Fatal(err)
	}
	loop:
	for {
		select {
		case <-exit:
			break loop
		default:
			listener.SetDeadline(time.Now().Add(1 * time.Second))
			conn, err := listener.Accept()
			if err != nil {
				errCont := err.(*net.OpError)
				if errCont.Timeout() {
					continue loop
				} else {
					log.Fatal(err)
				}
			}
			log.Println("accept " + conn.LocalAddr().String())
			go handleConnection(conn, exit)

		}
	}
}

func handleConnection(conn net.Conn, exit chan struct{}) {
	for {
		message := ""
		fmt.Fscanf(conn, "%s", &message)
		log.Println("recieved " + message)
		if message == "quit" {
			conn.Close()
			return
		} else if message == "exit" {
			conn.Close()
			exit <- struct{}{}
			return
		} else {
			fmt.Fprintf(conn, "%s\n", reverse(message))
		}
	}
}

func reverse(s string) string {
	len := len(s)
	ans := make([]rune, len)
	for i, x := range []rune(s) {
		ans[len - i - 1] = x
	}
	return string(ans)
}