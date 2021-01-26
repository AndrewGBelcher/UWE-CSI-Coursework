/**
 *
 */
import {
  Component,
  Input,
  ElementRef,
  OnInit } from '@angular/core';

import { SocketServiceComponent, Output } from '../socket-service/socket-service.component';

/**
 * This compontent manages the output and input from user to play game.
 * It is very simply, reads input, sends to server, listens for info, and
 * displays both user input and info from server in panel.
 *
 * @param  {'app-output-panel'}               {selector   [description]
 * @param  {'./output-panel.component.html'}  templateUrl [description]
 * @param  {['./output-panel.component.css']} styleUrls   [description]
 * @param  {[type]}                           }           [description]
 * @return {[type]}                                       [description]
 */
@Component({
  selector: 'app-output-panel',
  templateUrl: './output-panel.component.html',
  styleUrls: ['./output-panel.component.css'],
})
export class OutputPanelComponent implements OnInit {

  private prompt: string = "> ";

  private outputPanel: string[] = [];

  /**
   * [constructor description]
   * @param  {SocketServiceComponent} privatesocketService [description]
   * @return {[type]}                                      [description]
   */
  constructor(public socketService: SocketServiceComponent) {
    socketService.outputStream().subscribe(
      (msg : Output) => {
        this.handleOuput(msg.output);
      });
  }

  /**
   * [processInput description]
   * @param  {HTMLInputElement} msg [description]
   * @return {[type]}               [description]
   */
  processInput(msg: HTMLInputElement) {
      var strip : string = msg.value.substring(this.prompt.length);

      // send input to server
      this.socketService.send(strip);
      this.handleOuput(strip);
      // now reset prompt
      msg.value = this.prompt;
      document.getElementById( 'inputbox' ).scrollIntoView();
  }

  /**
   * [handleOuput description]
   * @param  {string} msg [description]
   * @return {[type]}     [description]
   */
  private handleOuput(msg : string) {
    this.outputPanel.push(msg);
    // finally scroll inputbox into view
  }

  /**
   * [ngOnInit description]
   * @return {[type]} [description]
   */
  ngOnInit() {
  }
}
