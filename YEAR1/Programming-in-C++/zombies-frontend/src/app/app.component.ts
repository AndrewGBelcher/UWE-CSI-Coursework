/**
 *
 */
import {
  Component,
  ElementRef, OnInit } from '@angular/core';

import {
  SocketServiceComponent,
  Score,
  Timer } from './socket-service/socket-service.component';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements OnInit {
  private nativeElement: HTMLElement;

  title : string = 'Zombies';
  score : number = 0;
  timerEnabled : boolean = false;
  timerValue : number = 0;
  timerOut : any;
  timerInterval : any;

  constructor(public socketService: SocketServiceComponent) {
    socketService.scoreStream().subscribe(
      (msg : Score) => {
          this.score = msg.score;
      });

    socketService.timerStream().subscribe(
        (msg : Timer) => {
            if (msg.value > 0) {
              if (!this.timerEnabled) {
                this.timerInterval = setInterval(() => {
                  this.timerValue--;
                }, 1000); // 1 sec

                this.timerOut = setTimeout(() => {
                  clearInterval(this.timerValue);
                  this.timerEnabled = false;
                  // timer expiered so we need to send message to server
                  this.socketService.send("timerexpired");
                }, msg.value * 1000); // server sends seconds

                this.timerEnabled = true;
                this.timerValue = msg.value;
              }
            }
            else {
              // be caeful timer might not enabled, so no clear stuff it is not
              if (this.timerEnabled) {
                this.timerEnabled = false;
                this.timerValue = 0;

                clearInterval(this.timerValue);
                clearTimeout(this.timerOut);
              }
            }
        });
  }

  ngOnInit() {
  }
}
