/**
 *
 */
import {
  Component,
  Input,
  ElementRef,
  OnInit } from '@angular/core';

import { ActivatedRoute, Router} from '@angular/router';

import { SocketServiceComponent, Output } from '../socket-service/socket-service.component';

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
  styleUrls: ['./home.component.css'],
})
export class HomeComponent  {

  /**
   * [constructor description]
   */
  constructor(
    private socketService: SocketServiceComponent) {
  }
}
